#!/bin/bash
# DESCRIPTION: Print the next patch version when the published package changed since the last tag (sources,
#              native code, bundled zstd, package.json entry points and peer deps); print nothing otherwise.
# USAGE: scripts/release-check.sh [ref]
# EXAMPLES:
#   scripts/release-check.sh            # checks origin/main
#   scripts/release-check.sh HEAD
set -euo pipefail

ref=${1:-origin/main}
prev=$(git describe --tags --abbrev=0 --match 'v*' "$ref")
shipped=(src cpp android ios nitrogen nitro.json ':(glob)*.podspec' react-native.config.js externals/zstd)

manifest() {
	git show "$1:package.json" | node -e '
		const p = JSON.parse(require("fs").readFileSync(0, "utf8"));
		console.log(JSON.stringify([p.main, p.types, p.exports, p.files, p.peerDependencies]));'
}

if git diff --quiet "$prev" "$ref" -- "${shipped[@]}" && [[ $(manifest "$prev") == "$(manifest "$ref")" ]]; then
	exit 0
fi

[[ ${prev#v} =~ ^([0-9]+)\.([0-9]+)\.([0-9]+)$ ]] || { echo "last tag is not a plain semver: $prev" >&2; exit 1; }
echo "${BASH_REMATCH[1]}.${BASH_REMATCH[2]}.$((BASH_REMATCH[3] + 1))"
