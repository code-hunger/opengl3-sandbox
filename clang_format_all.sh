git ls-files | grep -E '.*\.(h|cpp)' | xargs -I % sh -c 'clang-format --style=file % >tmp.txt; mv tmp.txt %;'
