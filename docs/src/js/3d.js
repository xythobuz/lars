import * as THREE from 'three';
import { OrbitControls } from 'three/addons/controls/OrbitControls.js';
import { STLLoader } from 'three/addons/loaders/STLLoader.js'
import { VRMLLoader } from 'three/addons/loaders/VRMLLoader.js';

export function init_3d(path, container, status, div_width, div_height) {
    const width = div_width;
    const height = div_height;

    const scene = new THREE.Scene();
    scene.add(new THREE.AxesHelper(1));

    const camera = new THREE.PerspectiveCamera( 75, width / height, 0.1, 1000 );

    const renderer = new THREE.WebGLRenderer();
    renderer.setSize( width, height );

    container.appendChild( renderer.domElement );

    const light = new THREE.DirectionalLight( 0xffffff, 0.5 );
    light.position.set(0, 1, 0);
    scene.add(light);

    const light42 = new THREE.DirectionalLight( 0xffffff, 0.5 );
    light42.position.set(1, 0, 0);
    scene.add(light42);

    const light23 = new THREE.DirectionalLight( 0xffffff, 0.5 );
    light23.position.set(0, 0, 1);
    scene.add(light23);

    const lightb = new THREE.DirectionalLight( 0xffffff, 0.5 );
    lightb.position.set(0, -1, 0);
    scene.add(lightb);

    const light42b = new THREE.DirectionalLight( 0xffffff, 0.5 );
    light42b.position.set(-1, 0, 0);
    scene.add(light42b);

    const light23b = new THREE.DirectionalLight( 0xffffff, 0.5 );
    light23b.position.set(0, 0, -1);
    scene.add(light23b);

    const light2 = new THREE.AmbientLight(0x101010);
    light2.position.set(100, 100, 100);
    scene.add(light2);

    const material = new THREE.MeshStandardMaterial();
    //material.roughness = 0.42;

    if (path.endsWith(".stl")) {
        const loader = new STLLoader();
        loader.load(
            //'plot/actuator_all.stl',
            path,
            function (geometry) {
                const mesh = new THREE.Mesh(geometry, material);
                scene.add(mesh);
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
        const loader = new VRMLLoader();
        loader.load(
            //'plot/drumkit.kicad_pcb.wrl',
            //'plot/dispensy.wrl',
            path,
            function (object) {
                scene.add(object);
        });
    } else {
        const s = "error: unknown filetype for " + path;
        console.log(s);
        status.textContent = s;
    }

    const controls = new OrbitControls( camera, renderer.domElement );
    controls.enableDamping = true;

    // https://wejn.org/2020/12/cracking-the-threejs-object-fitting-nut/
    const fitCameraToCenteredObject = function (camera, object, offset, orbitControls ) {
        const boundingBox = new THREE.Box3();
        boundingBox.setFromObject( object );

        var middle = new THREE.Vector3();
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
    };

    //camera.position.z = 50;
    fitCameraToCenteredObject(camera, scene, 0, controls)

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
