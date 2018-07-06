#!/bin/zsh

if [ $# -eq 0 ]
then
	echo "Usage: ./Norme.sh [ Directory/Path ]"
	exit;
fi

## operator/C define and include by clrichar
echo "_____________________________ OPERATOR AT THE END OF LINE ";
grep -REn "\+$" $1/**/*.[ch];
grep -REn "\+\s$" $1/**/*.[ch];
grep -REn "\-$" $1/**/*.[ch];
grep -REn "\-\s$" $1/**/*.[ch];
grep -REn "\*$" $1/**/*.[ch];
grep -REn "\*\s$" $1/**/*.[ch];
grep -REn " \/$" $1/**/*.[ch];
grep -REn " \/\s$" $1/**/*.[ch];
grep -REn "\%$" $1/**/*.[ch];
grep -REn "\%\s$" $1/**/*.[ch];
grep -REn "\=$" $1/**/*.[ch];
grep -REn "\=\s$" $1/**/*.[ch];
grep -REn "\|$" $1/**/*.[ch];
grep -REn "\|\s$" $1/**/*.[ch];
grep -REn "\&$" $1/**/*.[ch];
grep -REn "\&\s$" $1/**/*.[ch];
grep -RRv ".h>" $1/**/*.[ch] | grep -En ">$"
grep -RRv ".h>" $1/**/*.[ch] | grep -En ">\s$"
grep -REn "<$" $1/**/*.[ch];
grep -REn "<\s$" $1/**/*.[ch];
grep -REn "\^$" $1/**/*.[ch];
grep -REn "\^\s$" $1/**/*.[ch];
grep -REn "\~$" $1/**/*.[ch];
grep -REn "\~\s$" $1/**/*.[ch];

echo "_________________________________________________________ ";
echo "" 
echo "_________________________________________________________ ";

echo ""
echo "_________________________________________ DEFINE IN C FILE";
grep -En "\#*.*define" $1/**/*.c
echo "_________________________________________________________ ";
echo "________________________________________  C FILE INCLUSION";
grep -En '\.c"' $1/**/*.c

echo "_________________________________________________________ ";
echo "" 
echo "_________________________________________________________ ";


##	by kehuang
echo "_____________________________________ MULTIPLE ASSIGNATION";
grep -REn "\(*.t_.*\).*\{.*" $1/**/*.c;
grep -REn "\(*._t.*\).*\{.*" $1/**/*.c;
grep -REn "\(*.int.*\).*\{.*" $1/**/*.c;
grep -REn "\(*.char.*\).*\{.*" $1/**/*.c;
grep -REn "\(*.double.*\).*\{.*" $1/**/*.c;
grep -REn "\(*.float.*\).*\{.*" $1/**/*.c;
grep -REn "\(*.short.*\).*\{.*" $1/**/*.c;

echo "_________________________________________________________ ";
echo ""
echo "_________________________________________________________ ";

echo "_____________________________________________ DEFINE CONST";

echo "operation:"
grep -REn "\#.*.define.*.[-+*/%].*" $1/**/*.h;

echo "const:"
grep -REn "\#*.*define" $1/**/*.h | grep -v "[\"\'0-9A-Z]$"\
	| grep "define*.*[a-z_]";

echo "_________________________________________________________ ";
echo ""
echo "_________________________________________________________ ";

##	check VLA ... No shit sherlock
echo "______________________________________________________ VLA";
grep -REn "\[\]" $1/**/*.[ch];

echo "_________________________________________________________ ";
echo ""
echo "_________________________________________________________ ";

##	check attribution string in stack with only a pointer
##	=>	char *str; str = "string";
echo "___________________________________________ ILLEGAL STRING";
grep -REn "\= \"*.*\";" $1/**/*.c | grep -v "static*.*\=" | grep -v "g_*.*\=";
echo "_________________________________________________________ ";
