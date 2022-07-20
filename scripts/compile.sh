#!/bin/bash

# Nothing interesting to see here, script copied from somewhere used to skip clang-tidy on certain files

command="$@"

if [[ "$command" =~ .*[[:space:]](.+) ]]; then
    filepath="${BASH_REMATCH[1]}"
    while read pattern; do
        if [[ "$filepath" =~ $pattern ]]; then
            command="$(echo "$command" | sed 's/.* -- //g')"
            break
        fi
    done < "$(cd "$( dirname "${BASH_SOURCE[0]}")" && pwd )"/../.clang-tidy-exclude
fi

$command
