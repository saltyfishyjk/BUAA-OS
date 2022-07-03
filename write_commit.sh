echo make clean > commit.sh
echo git add -A >> commit.sh
echo git commit -a -m \" $1 \" >> commit.sh
echo git push origin $2:$2 >> commit.sh
