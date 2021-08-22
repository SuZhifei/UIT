#!/bin/bash

printInfo() {
    echo -e "\033[0;36mINFO:\033[0;0m $*"
}

printWarn() {
    echo -e "\033[0;33mWARN:\033[0;0m $*"
}

printError() {
    echo -e "\033[0;31mERROR:\033[0;0m $*"
}

isValidBaseline() {
	res=$(git --git-dir=$1/.git --work-tree=$1 branch -r | grep "origin/$2$")
    if [ "$2" != "master" ] && [ $res ]; then
		return 0;
	else
		return 1;
	fi
}

setBaselineOne() {
    if isValidBaseline $1 $2; then
        res=$(git --git-dir=$1/.git --work-tree=$1 checkout $2 2>/dev/null)
        if [ $? -eq 0 ]; then
            printInfo "setBaseline of $1 to $2"
			return 0;
        else
            printError "setBaseline of $1 to $2 fail, maybe you have uncommit changes."
			return 1;
        fi
    else
        printError "$1 has no baseline $2!"
		return 1;
    fi
}

setBaseline() {
    if ! setBaselineOne $UIT_ROOT $1; then
        return 0;
    fi

    export UIT_BASELINE=$1

    for m in $UIT_ROOT/package/*; do
        if [ -d $m/.git ]; then
			setBaselineOne $m $1
        fi
    done
}

