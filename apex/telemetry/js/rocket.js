import {
    STLLoader
} from 'https://cdn.jsdelivr.net/npm/three@0.120.1/examples/jsm/loaders/STLLoader.js';

let canvas = document.getElementById('canvas');

// Setup
let scene = new THREE.Scene();
scene.background = new THREE.Color(0x2e3131);
let cHeight = canvas.clientHeight * 3.5;
let cWidth = canvas.clientWidth * 2.5;
let camera = new THREE.PerspectiveCamera(75, cWidth / cHeight, 0.1, 10000);
let renderer = new THREE.WebGLRenderer({canvas: canvas, antialias: true});

renderer.setSize(cWidth, cHeight);
console.log(renderer.domElement)
// canvas.appendChild(renderer.domElement);

// Resize after viewport-size-change
window.addEventListener("resize", function () {
    let height = cHeight;
    let width = cWidth;
    renderer.setSize(width, height);
    camera.aspect = width / height;
    camera.updateProjectionMatrix();
});

let loader = new STLLoader();
loader.load('./apex.stl', function (geometry) {
    console.log(geometry);
    let material = new THREE.MeshBasicMaterial({
        color: 0x6c7a89,
    });
    let mesh = new THREE.Mesh(geometry, material);
    scene.add(mesh);
    geometry.center();
    geometry.translate(1, 0, 0); //Account for bounding box center error
    camera.lookAt(mesh.position);

    window.set_euler = function set_euler(x, y, z) {
        mesh.rotation.x = x;
        mesh.rotation.y = y;
        mesh.rotation.z = z;
    }
});

const axesHelper = new THREE.AxesHelper(50);
scene.add(axesHelper);

// Camera positioning
camera.position.x = 72 * 0.7;
camera.position.y = 25 * 0.7;
camera.position.z = 100 * 0.7;

// Draw scene
let render = function () {
    renderer.render(scene, camera);
};

// Run game loop (render,repeat)
let GameLoop = function () {
    requestAnimationFrame(GameLoop);
    render();
};

GameLoop();