git ls-files | grep -v '^third_party' | grep -E '.*\.(h|cpp)' | xargs -I % sh -c 'clang-format --style=file % >tmp.txt; mv tmp.txt %;'
