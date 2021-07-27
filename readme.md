git init
touch readme.md
git add .
git commit -m "repo: initial commit"

git checkout -b first_branch
git commit -am "readme: added the command log for the first task"

git stash
git checkout master
git log --oneline --decorate --graph --all
git merge first_branch
git stash pop

git log --oneline --decorate --graph --all
git commit -am "readme: add command log to solve 2nd subtask"
