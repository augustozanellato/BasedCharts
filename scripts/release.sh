#!/usr/bin/env sh
# shellcheck disable=SC2164
cd "$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )/.."
(
  cd docs || exit
  latexmk
)
rm -f BasedCharts.zip
zip -r BasedCharts.zip BasedCharts.pro src/ examples/ assets/ -x src/CMakeLists.txt
zip -jr BasedCharts.zip docs/relazione.pdf