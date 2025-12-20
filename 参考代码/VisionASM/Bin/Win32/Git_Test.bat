set TT=%date:~0,4%/%date:~5,2%/%date:~8,2% %time:~0,2%:%time:~3,2%:%time:~6,2%
echo [%TT%]Begin Commit>>D:\\CommitLog.log 2>&1
Set P=%~dp0
echo Current Path:[%P%]>> D:\\CommitLog.log 2>&1

echo Current Branch:>> D:\\CommitLog.log 2>&1
git symbolic-ref --short -q HEAD>> D:\\CommitLog.log 2>&1

echo Add:>> D:\\CommitLog.log 2>&1
git add .

echo commit:%1%>> D:\\CommitLog.log 2>&1
git commit -m "%TT%_%1%"

echo Commit Success!>> D:\\CommitLog.log 2>&1

exit
