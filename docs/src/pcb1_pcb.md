# Prototype PCB Layout

This page shows the current version of the PCB layout as SVG graphics.

## 2D PCB Layout

You can also view the [2D PCB layout as PDF](./plot/drumkit.kicad_pcb.pdf).

<script src="js/svg-pan-zoom.js" charset="UTF-8"></script>
<div style="background-color: white; border: 1px solid black;">
    <embed type="image/svg+xml" src="./plot/drumkit.kicad_pcb.svg" id="pz_drumkit0" style="width: 100%;"/>
    <script>
        document.getElementById('pz_drumkit0').addEventListener('load', function(){
            svgPanZoom(document.getElementById('pz_drumkit0'), {controlIconsEnabled: true, minZoom: 1.0});
        })
    </script>
</div>

[Direct link to this file](./plot/drumkit.kicad_pcb.svg).

## DIY Version

You can also view the [DIY layout as PDF](./plot/drumkit.kicad_pcb_diy.pdf).

<script src="js/svg-pan-zoom.js" charset="UTF-8"></script>
<div style="background-color: white; border: 1px solid black;">
    <embed type="image/svg+xml" src="./plot/drumkit.kicad_pcb_diy.svg" id="pz_drumkit1" style="width: 100%;"/>
    <script>
        document.getElementById('pz_drumkit1').addEventListener('load', function(){
            svgPanZoom(document.getElementById('pz_drumkit1'), {controlIconsEnabled: true, minZoom: 1.0});
        })
    </script>
</div>

[Direct link to this file](./plot/drumkit.kicad_pcb_diy.svg).

## 3D PCB Layout

<p>Status: "<span id="3d_info">Preparing 3D model...</span>"</p>
<div id="3d_viewer" style="width: 100%; height: 100%; background-color: white; border: 1px solid black;"></div>

<script type="importmap">
    {
        "imports": {
            "three": "https://cdn.jsdelivr.net/npm/three@0.163.0/build/three.module.js",
            "three/addons/": "https://cdn.jsdelivr.net/npm/three@0.163.0/examples/jsm/"
        }
    }
</script>

<script type="module">
    var info = document.getElementById('3d_info');
    var view = document.getElementById('3d_viewer');
    view.style.height = Math.floor(view.clientWidth * 0.707) + "px";

    import { init_3d } from "./js/3d.js";
    init_3d('plot/drumkit.kicad_pcb.wrl', view, info, view.clientWidth, view.clientHeight);
</script>
