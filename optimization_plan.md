# VisionForge Pro Optimization Plan

This package contains the source files and instructions to optimize the VisionForge Pro project.

## 1. Memory Management Optimization (ImageMemoryPool)

**Goal**: Implement RAII for `ImageData` to ensure memory is automatically returned to the pool when `shared_ptr` reference count drops to zero.

**Files**:
- `include/base/ImageMemoryPool.h`
- `src/base/ImageMemoryPool.cpp`

**Instructions**:

1.  **Modify `ImageMemoryPool::allocate` in `src/base/ImageMemoryPool.cpp`**:
    Change the `std::make_shared` call to use a custom deleter.

    ```cpp
    // Old
    // return std::make_shared<ImageData>(width, height, type);

    // New
    ImageData* ptr = new ImageData(width, height, type);
    return std::shared_ptr<ImageData>(ptr, [this](ImageData* p) {
        this->release(std::shared_ptr<ImageData>(p, [](ImageData*){})); // Prevent double deletion in release
        // Note: The release method expects a shared_ptr.
        // A better approach for the custom deleter is to have a private releaseRaw(ImageData*) method
        // or modify release to accept raw pointer or handle the re-wrapping carefully.
    });
    ```

    *Correction*: A cleaner way is to have `allocate` return a `shared_ptr` that, when destroyed, calls a specific return-to-pool function.
    However, `release` currently takes `ImageData::Ptr` (which is `shared_ptr`).
    If we use a custom deleter, the `shared_ptr` manages the raw pointer. When ref count hits 0, the deleter is called with the raw pointer.
    We should change `release` to accept `ImageData*` or create a private helper.

    **Refined Implementation**:
    
    In `src/base/ImageMemoryPool.cpp`:

    ```cpp
    ImageData::Ptr ImageMemoryPool::allocate(int width, int height, int type)
    {
        // ... (existing logic to find in pool) ...
        
        if (found_in_pool) {
            // ... (pop from pool) ...
            // The object in pool is already a shared_ptr, but we need to re-attach the custom deleter
            // if it was stored with one, or just return it. 
            // Actually, if we store shared_ptr in the pool, they hold a reference.
            // When we give it out, we return that shared_ptr.
            // BUT, we want the deleter to trigger "return to pool" ONLY when the user is done.
            
            // Strategy: The pool stores raw pointers or unique_ptrs internally, 
            // and dispenses shared_ptrs with custom deleters.
            
            // Let's stick to the current pool storage (vector<ImageData::Ptr>).
            // When we take one out, we need to return a NEW shared_ptr that points to the same object,
            // but with a deleter that puts it back.
            // This is tricky with shared_from_this or if we want to reuse the control block.
            
            // Simpler Strategy for this codebase:
            // Just ensure that when we create a NEW ImageData, we attach the deleter.
            // When we recycle, we just return the existing shared_ptr? No, that won't trigger deleter again.
            
            // BEST APPROACH:
            // 1. Change pool storage to `std::vector<ImageData*>` (raw pointers).
            // 2. `allocate` returns `std::shared_ptr<ImageData>(raw_ptr, [this](ImageData* p){ this->release(p); });`
            // 3. `release(ImageData* p)` puts it back into the vector.
        }
        
        // If not found:
        ImageData* ptr = new ImageData(width, height, type);
        return std::shared_ptr<ImageData>(ptr, [this](ImageData* p) {
            this->release(p);
        });
    }
    ```

    **Action**:
    1.  Update `include/base/ImageMemoryPool.h`:
        - Change `std::map<PoolKey, std::vector<ImageData::Ptr>> pool_;` to `std::map<PoolKey, std::vector<ImageData*>> pool_;`
        - Change `void release(ImageData::Ptr image);` to `void release(ImageData* image);` (make it private or public as needed).
    2.  Update `src/base/ImageMemoryPool.cpp`:
        - Update `allocate` to use custom deleter.
        - Update `release` to handle raw pointer and add to pool.
        - Update destructor `~ImageMemoryPool` to delete raw pointers in the pool.

## 2. Parallel Processor Optimization

**Goal**: Optimize reduction operations (`parallelMax`, `parallelMin`) to avoid `critical` sections which kill performance.

**Files**:
- `src/base/ParallelProcessor.cpp`

**Instructions**:

1.  **Modify `parallelMax`**:
    Use a thread-local variable and a final reduction.

    ```cpp
    double ParallelProcessor::parallelMax(const double* data, size_t size)
    {
        if (size == 0) return 0.0;
        double globalMax = data[0];

    #ifdef _OPENMP
        #pragma omp parallel
        {
            double localMax = -std::numeric_limits<double>::infinity();
            
            #pragma omp for nowait
            for (int i = 0; i < static_cast<int>(size); ++i) {
                if (data[i] > localMax) {
                    localMax = data[i];
                }
            }
            
            #pragma omp critical
            {
                if (localMax > globalMax) {
                    globalMax = localMax;
                }
            }
        }
    #else
        // Serial implementation
    #endif
        return globalMax;
    }
    ```
    *Note*: The original code already had a similar structure but initialized `localMax` to `data[0]`. If the array is split, `data[0]` might not be in the thread's range. Initializing to `-infinity` is safer.

## 3. Asynchronous Batch Processing

**Goal**: Move batch image processing off the main UI thread to prevent freezing.

**Files**:
- `include/ui/MainWindow.h`
- `src/ui/MainWindow.cpp`

**Instructions**:

1.  **Define `InspectionWorker` class** (can be in `MainWindow.cpp` or separate file):
    It should inherit `QThread` or `QObject` and have a `process()` method.

2.  **Update `MainWindow::onRunAllImages`**:
    - Instead of a for-loop with `processEvents`, instantiate `InspectionWorker`.
    - Connect signals: `progress(int, int)`, `imageProcessed(int, bool)`, `finished()`.
    - Start the worker.
    - Disable UI controls while running.

    ```cpp
    // Pseudo-code for MainWindow.cpp
    
    void MainWindow::onRunAllImages() {
        // ... setup ...
        
        // Disable UI
        this->setEnabled(false);
        
        // Create worker
        auto worker = new InspectionWorker(imageSequence_, toolChainPanel_->getTools());
        
        connect(worker, &InspectionWorker::progress, this, [](int current, int total){
            // Update status bar
        });
        
        connect(worker, &InspectionWorker::finished, this, [this, worker](){
            this->setEnabled(true);
            worker->deleteLater();
            QMessageBox::information(this, "Finished", "Batch processing complete.");
        });
        
        worker->start();
    }
    ```

