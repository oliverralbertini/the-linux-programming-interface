#!/bin/sh

OFFSET=100000000
STRING="Hello World!"

# pathname offset string
echo "Creating file \"abc\" with offset: $OFFSET, and string \"$STRING\""
./ex.seek_write abc $OFFSET "$STRING"

echo "Size of \"abc\":"
du -h abc

echo "copying \"abc\" to \"def\" using \`cat\`"
cat abc > def

echo "Size of \"def\":"
du -h def

echo "copying \"abc\" to \"def\" using \`cp\`"
cp abc def

echo "Size of \"def\":"
du -h def

echo "copying \"abc\" to \"def\" using \`./copy\`"
./copy abc def

echo "Size of \"def\":"
du -h def

rm abc def
