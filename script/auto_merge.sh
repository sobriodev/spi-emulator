#!/bin/bash

commit_message=$TRAVIS_COMMIT_MESSAGE
commit_message_regex="\(\s\+\|^\)\([cC]loses\?\|[rR]esolves\?\|[fF]ix\(es\)\?\) #[0-9]\+"

echo Commit message is:
echo "$commit_message"
 
if echo "$commit_message" | grep -q "$commit_message_regex"; then
    curl -o /tmp/travis-automerge https://raw.githubusercontent.com/cdown/travis-automerge/master/travis-automerge
    chmod a+x /tmp/travis-automerge
    BRANCHES_TO_MERGE_REGEX="" BRANCH_TO_MERGE_INTO=develop GITHUB_REPO=sobriodev/spi-emulator /tmp/travis-automerge
else
    echo "Commit message does not fulfill merge requirements"
fi
