https://pages.github.com/

UPDATE <username>.github.io
 git add -A             #--all
 git commit -m 'all'
 #git remote add origin https://github.com/<username>/<username>.github.io
 git push origin master # popup deny 

CHANGE NAME & EMAIL
 git config --global user.email "name@gmail.com"
 git config --global user.name "name"
 git config --global --list

CONFIGURE GIT
 nano .git/config 

GHPAGES to MASTER
 git checkout gh-pages
 git merge master
 git push origin gh-pages

echo "# dplyme.github.io" >> README.md
git init
git add README.md
git commit -m "1st"
git remote add origin https://github.com/<username>/dplyme.git
git push origin master

git init
git add README.md
git commit -m "1st commit"
git add remote origin url https://github.com/<username>/<username>.github.io.git
git push -u origin master

git add .
git commit -m "2nd commit"
git remote add origin https://github.com/<username>/dplyd-me.git
git pull origin master (pull all commits from master so not to overridde)
git push origin master

