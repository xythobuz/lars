/*
 * 3d.js
 *
 * Copyright (c) 2024 Thomas Buck (thomas@xythobuz.de)
 */

import * as THREE from 'three';
import { OrbitControls } from 'three/addons/controls/OrbitControls.js';
import { STLLoader } from 'three/addons/loaders/STLLoader.js'
import { VRMLLoader } from 'three/addons/loaders/VRMLLoader.js';

// https://wejn.org/2020/12/cracking-the-threejs-object-fitting-nut/
function fitCameraToCenteredObject(camera, object, offset, orbitControls ) {
    const boundingBox = new THREE.Box3();
    boundingBox.setFromObject( object );

    var size = new THREE.Vector3();
    boundingBox.getSize(size);

    // figure out how to fit the box in the view:
    // 1. figure out horizontal FOV (on non-1.0 aspects)
    // 2. figure out distance from the object in X and Y planes
    // 3. select the max distance (to fit both sides in)
    //
    // The reason is as follows:
    //
    // Imagine a bounding box (BB) is centered at (0,0,0).
    // Camera has vertical FOV (camera.fov) and horizontal FOV
    // (camera.fov scaled by aspect, see fovh below)
    //
    // Therefore if you want to put the entire object into the field of view,
    // you have to compute the distance as: z/2 (half of Z size of the BB
    // protruding towards us) plus for both X and Y size of BB you have to
    // figure out the distance created by the appropriate FOV.
    //
    // The FOV is always a triangle:
    //
    //  (size/2)
    // +--------+
    // |       /
    // |      /
    // |     /
    // | F° /
    // |   /
    // |  /
    // | /
    // |/
    //
    // F° is half of respective FOV, so to compute the distance (the length
    // of the straight line) one has to: `size/2 / Math.tan(F)`.
    //
    // FTR, from https://threejs.org/docs/#api/en/cameras/PerspectiveCamera
    // the camera.fov is the vertical FOV.

    const fov = camera.fov * ( Math.PI / 180 );
    const fovh = 2*Math.atan(Math.tan(fov/2) * camera.aspect);
    let dx = size.z / 2 + Math.abs( size.x / 2 / Math.tan( fovh / 2 ) );
    let dy = size.z / 2 + Math.abs( size.y / 2 / Math.tan( fov / 2 ) );
    let cameraZ = Math.max(dx, dy);

    // offset the camera, if desired (to avoid filling the whole canvas)
    if( offset !== undefined && offset !== 0 ) cameraZ *= offset;

    camera.position.set( 0, 0, cameraZ );

    // set the far plane of the camera so that it easily encompasses the whole object
    const minZ = boundingBox.min.z;
    const cameraToFarEdge = ( minZ < 0 ) ? -minZ + cameraZ : cameraZ - minZ;

    camera.far = cameraToFarEdge * 3;
    camera.updateProjectionMatrix();

    if ( orbitControls !== undefined ) {
        // set camera to rotate around the center
        orbitControls.target = new THREE.Vector3(0, 0, 0);

        // prevent camera from zooming out far enough to create far plane cutoff
        orbitControls.maxDistance = cameraToFarEdge * 2;
    }
}

export function init_3d(path, container, status, div_width, div_height) {
    const width = div_width;
    const height = div_height;

    const scene = new THREE.Scene();
    scene.add(new THREE.AxesHelper(1));

    const camera = new THREE.PerspectiveCamera( 75, width / height, 0.1, 1000 );

    const renderer = new THREE.WebGLRenderer();
    renderer.setSize( width, height );

    container.appendChild( renderer.domElement );

    const controls = new OrbitControls( camera, renderer.domElement );
    controls.enableDamping = true;

    if (path.endsWith(".stl")) {
        const light_amb = new THREE.AmbientLight(0x424242);
        scene.add(light_amb);

        for (const i of [1, -1]) {
            for (const j of [0, 1, 2]) {
                const light = new THREE.DirectionalLight(0xffffff, 0.5);
                light.position.set(i * (j == 0 ? 1 : 0),
                                   i * (j == 1 ? 1 : 0),
                                   i * (j == 2 ? 1 : 0));
                scene.add(light);
            }
        }

        const material = new THREE.MeshStandardMaterial();
        //material.roughness = 0.75;

        const loader = new STLLoader();
        loader.load(
            path,
            function (geometry) {
                const mesh = new THREE.Mesh(geometry, material);
                scene.add(mesh);
                fitCameraToCenteredObject(camera, scene, 0, controls);
            },
            (xhr) => {
                const s = (xhr.loaded / xhr.total) * 100 + '% loaded';
                console.log(s);
                status.textContent = s;
            },
            (error) => {
                console.log(error);
                status.textContent = error;
            }
        );
    } else if (path.endsWith(".wrl")) {
        const light_amb = new THREE.AmbientLight(0xffffff);
        scene.add(light_amb);

        const loader = new VRMLLoader();
        loader.load(
            path,
            function (object) {
                scene.add(object);
                fitCameraToCenteredObject(camera, scene, 0, controls);
            },
            (xhr) => {
                const s = (xhr.loaded / xhr.total) * 100 + '% loaded';
                console.log(s);
                status.textContent = s;
            },
            (error) => {
                console.log(error);
                status.textContent = error;
            }
        );
    } else {
        const s = "error: unknown filetype for " + path;
        console.log(s);
        status.textContent = s;
    }

    camera.position.z = 50;

    function render() {
        renderer.render(scene, camera);
    }

    function animate() {
        requestAnimationFrame(animate);
        controls.update();
        render();
    }

    animate();
    status.textContent = "3D model ready!";
}
