#!/bin/bash

# SPDX-FileCopyrightText: 2024 Thomas Buck <thomas@xythobuz.de>
# SPDX-License-Identifier: CERN-OHL-S-2.0+
#
#  ------------------------------------------------------------------------------
# | Copyright (c) 2024 Thomas Buck <thomas@xythobuz.de>                          |
# |                                                                              |
# | This source describes Open Hardware and is licensed under the CERN-OHL-S v2  |
# | or any later version.                                                        |
# |                                                                              |
# | You may redistribute and modify this source and make products using it under |
# | the terms of the CERN-OHL-S v2 (https://ohwr.org/cern_ohl_s_v2.txt)          |
# | or any later version.                                                        |
# |                                                                              |
# | This source is distributed WITHOUT ANY EXPRESS OR IMPLIED WARRANTY,          |
# | INCLUDING OF MERCHANTABILITY, SATISFACTORY QUALITY AND FITNESS FOR A         |
# | PARTICULAR PURPOSE. Please see the CERN-OHL-S v2 (or any later version)      |
# | for applicable conditions.                                                   |
# |                                                                              |
# | Source location: https://git.xythobuz.de/thomas/drumkit                      |
# |                                                                              |
# | As per CERN-OHL-S v2 section 4, should You produce hardware based on this    |
# | source, You must where practicable maintain the Source Location visible      |
# | on the external case of the Gizmo or other products you make using this      |
# | source.                                                                      |
#  ------------------------------------------------------------------------------

INSCH="drumkit.kicad_sch"
INPCB="drumkit.kicad_pcb"

cd "$(dirname "$0")"

echo "Generating plots"
rm -rf src/plot
../pcb/generate_plot.sh
cp -r ../pcb/plot src
echo

for IN in $INSCH
do
    o="src/inc_$IN.md"
    echo "Include for $IN at $o"

    rm -rf $o
    echo "<script src=\"js/svg-pan-zoom.js\" charset=\"UTF-8\"></script>" >> $o

    for f in `ls src/plot/$IN.svg/*.svg | sort -r`; do
        file=`echo $f | sed 's:src/:./:g'`
        name=`echo $f | sed "s:src/plot/$IN.svg/::g" | sed 's:.svg::g'`
        echo "Sheet at $name"

        echo "<h2>$name</h2>" >> $o
        echo "<div style=\"background-color: white; border: 1px solid black;\">" >> $o
        echo "<embed type=\"image/svg+xml\" src=\"$file\" id=\"pz_$name\" style=\"width:100%;\"/>" >> $o
        echo "<script>" >> $o
        echo "document.getElementById('pz_$name').addEventListener('load', function(){" >> $o
        echo "svgPanZoom(document.getElementById('pz_$name'), {controlIconsEnabled: true, minZoom: 1.0});" >> $o
        echo "})" >> $o
        echo "</script>" >> $o
        echo "</div>" >> $o
        echo >> $o
        echo "[Direct link to \`$name\`]($file)." >> $o
        echo >> $o
    done

    echo
done

echo "Generating docs"
if [ "$1" = "serve" ] ; then
    mdbook serve --open
elif [ "$1" = "build" ] ; then
    mdbook build
else
    echo "Invalid command. 'build' or 'serve'."
fi
