#!/bin/bash

# Disable pathname expansion
set -f

let REPO="package"

printInfo() {
    echo -e "\033[0;0m[\033[0;36m $REPO \033[0;0m] \033[0;32mINFO\033[0;0m: $1"
}

printWarn() {
    echo -e "\033[0;0m[\033[0;36m $REPO \033[0;0m] \033[0;33mWARN\033[0;0m: $1"
}

printError() {
    echo -e "\033[0;0m[\033[0;36m $REPO \033[0;0m] \033[0;31mERROR\033[0;0m: $1"
}

printErrorAndExit() {
    local exit_code=$?
    printError "$1"
    exit $exit_code
}

isGitRepo() {
    [ -e ".git" ] && git branch > /dev/null
}

isGitDirty() {
    [ -n "$(git status -s)" ]
}

doGitFetch() {
    git fetch -p 2>&1 | while read output; do
        printInfo "$output"
    done
}

doMakeMaster() {
    git checkout "$UIT_BASELINE" 2>/dev/null 1> /dev/null
}

getCurrentBranch() {
    git symbolic-ref --short -q HEAD
}

hasRemoteBranch() {
    # $1 branch name
    for name in $(git branch -a); do
        if [ "$name" == "remotes/origin/$1" ]; then
            return 0 # direct return
        fi
    done
    [ "$?" !=  "0" ]
}

hasLocalBranch() {
    # $1 branch name
    for name in $(git branch); do
        if [ "$name" == "$1" ]; then
            return 0 # direct return
        fi
    done
    [ "$?" !=  "0" ]
}

isDirtyBranch() {
    # $1 branch name
    hasRemoteBranch "$1" || return 1

    local dirty=$(git cherry origin/$1 $1)
    [ -n "$dirty" ]
}

isBackupBranch() {
    # $1 branch name
    # backup_flag="_backup_123456789012", length=1+6+1+12=20
    local branch=${1%%_backup_????????????}
    local date_seconds=${1##$branch"_backup_"}
    [ ${#branch} -ne ${#1} ] && date -d @$date_seconds 2> /dev/null 1> /dev/null
}

isRemoteDeletedBranch() {
    # $1 branch name
    git fetch -p --dry-run 2>&1 | while read -a update; do
        if ([ "${update[0]}" == "-" ] || [ "${update[0]}" == "x" ]) &&
            [ "${update[4]}" = "origin/$1" ]; then
            return 1
    fi
    done
    [ $? != "0" ]
}

doBackupBranch() {
    # $1 branch to backup
    local source_branch=$1
    local backup_branch=$1_backup_$(date "+%012s")

    printInfo "backup \"$source_branch\" to \"$backup_branch\" ..."

    git checkout -b $backup_branch origin/$source_branch 1> /dev/null

    git cherry origin/$source_branch $source_branch | while read -a commit; do
        git cherry-pick ${commit[1]}
    done

    git branch -D $source_branch

    git branch --unset-upstream
}

doUpdateLocalBranch() {
    # $1 branch name
    git checkout $1 2> /dev/null 1> /dev/null
    git pull --rebase | while read output; do
        printInfo "$output" > /dev/null
    done
}

doUpdateBackupBranch() {
    # $1 branch name
    git checkout $1
    git rebase origin/master
}

doCleanBackupBranch() {
    # $1 branch name
    printInfo "Clean $1"
    local branch=${1%%_backup_????????????}
    git branch -m $1 $branch
}

tryMakeMaster() {
    hasRemoteBranch "$UIT_BASELINE" || printErrorAndExit "Empty git repo, manual commit first pls."

    isDirtyBranch "$UIT_BASELINE" && printErrorAndExit "Commit to master is not allowed!"

    doMakeMaster
}

tryBackupLocalBranch() {
    # $1 local branch name
    [ "$1" == "*" ] && return 0

    hasRemoteBranch $1 || return 0

    isDirtyBranch $1 || return 0

    isRemoteDeletedBranch "$1" || return 0

    printInfo "$1 deleted remotely, backup unpushed commits ..."
    doBackupBranch $1 $backup_branch
}

tryBackupLocalBranches() {
    for branch in $(git branch); do
        tryBackupLocalBranch $branch
    done
}

tryUpdateLocalBranches() {
    for branch in $(git branch); do
        hasRemoteBranch $branch || continue
        doUpdateLocalBranch $branch || printErrorAndExit "Update $branch fail! Pls try update manually."
    done
}

abortUpdateBackupBranch() {
    git rebase --abort
}

tryUpdateBackupBranches() {
    for branch in $(git branch); do
        isBackupBranch $branch || continue
        doUpdateBackupBranch $branch
    done
}

tryCleanBackupBranches() {
    for branch in $(git branch); do
        isBackupBranch $branch || continue
        doCleanBackupBranch $branch
    done
}

[ $# -eq 1 ] || printErrorAndExit "Pls input path of git repo to update!"

[ -n "$UIT_BASELINE" ] || printErrorAndExit "Pls setBaseline first!"

REPO=$1

cd $REPO

#printInfo "Updating ..."

isGitRepo || printErrorAndExit "Invalid git repo!"

isGitDirty && doGitFetch && printErrorAndExit "Have unstaged or uncommitted changes, pls commit or stash them first!"

tryMakeMaster || printErrorAndExit "Make master fail!"

tryBackupLocalBranches || printErrorAndExit "Backup local branches fail!"

doGitFetch || printErrorAndExit "Fetch fail!"

tryUpdateLocalBranches || printErrorAndExit "Update local branches fail!"

tryUpdateBackupBranches || printErrorAndExit "Update backup branches fail!"

tryMakeMaster

tryCleanBackupBranches || printErrorAndExit "Clean backup branches fail!"

#printInfo "Updated."

cd ..
