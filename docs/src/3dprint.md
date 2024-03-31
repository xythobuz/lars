# Mounting Hardware

**TODO** work in progress

<script type="importmap">
    {
        "imports": {
            "three": "https://cdn.jsdelivr.net/npm/three@0.163.0/build/three.module.js",
            "three/addons/": "https://cdn.jsdelivr.net/npm/three@0.163.0/examples/jsm/"
        }
    }
</script>

## Actuator All

<p>Status: "<span id="3d_info">Preparing 3D model...</span>"</p>
<div id="3d_viewer" style="width: 100%; height: 100%; background-color: white; border: 1px solid black;"></div>

<script type="module">
    var info = document.getElementById('3d_info');
    var view = document.getElementById('3d_viewer');
    view.style.height = Math.floor(view.clientWidth * 0.707) + "px";

    import { init_3d } from "./js/3d.js";
    init_3d('stl/actuator_all.stl', view, info, view.clientWidth, view.clientHeight);
</script>

## Actuator Cap

<p>Status: "<span id="3d_info2">Preparing 3D model...</span>"</p>
<div id="3d_viewer2" style="width: 100%; height: 100%; background-color: white; border: 1px solid black;"></div>

<script type="module">
    var info = document.getElementById('3d_info2');
    var view = document.getElementById('3d_viewer2');
    view.style.height = Math.floor(view.clientWidth * 0.707) + "px";

    import { init_3d } from "./js/3d.js";
    init_3d('stl/actuator_cap.stl', view, info, view.clientWidth, view.clientHeight);
</script>
