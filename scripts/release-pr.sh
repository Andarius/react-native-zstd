#!/bin/bash
# DESCRIPTION: Open a release PR (release/<version>): bumps package.json, drafts notes from the commits
#              since the last tag. Merging the PR publishes to npm and creates the GitHub release;
#              the PR description becomes the release notes, so edit it before merging.
# USAGE: scripts/release-pr.sh <version>
# EXAMPLES:
#   scripts/release-pr.sh 1.5.0
set -euo pipefail

version=${1:?usage: scripts/release-pr.sh <version>}
version=${version#v}
[[ $version =~ ^[0-9]+\.[0-9]+\.[0-9]+(-[0-9A-Za-z.]+)?$ ]] || { echo "not a semver: $version" >&2; exit 1; }
[[ -z $(git status --porcelain --untracked-files=no) ]] || { echo "working tree has changes" >&2; exit 1; }

git fetch -q origin main --tags
prev=$(git describe --tags --abbrev=0 --match 'v*' origin/main 2>/dev/null || true)
notes=$(git log --no-merges --format='- %s' "${prev:+$prev..}origin/main" | grep -vE '^- chore(\(release\))?: release ' || true)
[[ -n $notes ]] || { echo "nothing to release since ${prev:-the first commit}" >&2; exit 1; }

git switch -q --no-track -c "release/$version" origin/main
npm version "$version" --no-git-tag-version --allow-same-version >/dev/null
if [[ -f CHANGELOG.md ]]; then
	# new entry goes above the latest "## " section, i.e. under the "# Changelog" title
	entry=$(mktemp)
	printf '## %s (%s)\n\n%s\n\n' "$version" "$(date +%Y-%m-%d)" "$notes" >"$entry"
	awk -v f="$entry" '!done && /^## / { while ((getline l < f) > 0) print l; done = 1 } { print }
		END { if (!done) { print ""; while ((getline l < f) > 0) print l } }' CHANGELOG.md >CHANGELOG.md.tmp
	mv CHANGELOG.md.tmp CHANGELOG.md
	rm -f "$entry"
fi
git commit -qam "chore: release $version"
git push -q -u origin "release/$version"
gh pr create --base main --head "release/$version" --title "chore: release $version" --body "$notes"
