#!/bin/sh
cd "$(dirname "$0")/.."
mkdir -p external
cd external 
git clone http://git.chromium.org/external/googletest.git
