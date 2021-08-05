import { STLLoader } from 'https://cdn.jsdelivr.net/npm/three@0.120.1/examples/jsm/loaders/STLLoader.js';

let canvas = document.getElementById('canvas');
document.body.appendChild(canvas);

// Setup
let scene = new THREE.Scene();
scene.background = new THREE.Color(0x2e3131);
let camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 10000);
let renderer = new THREE.WebGLRenderer();

renderer.setSize(window.innerWidth, window.innerHeight);
console.log(renderer.domElement)
document.body.appendChild(renderer.domElement);

// Resize after viewport-size-change
window.addEventListener("resize", function () {
    let height = window.innerHeight;
    let width = window.innerWidth;
    renderer.setSize(width, height);
    camera.aspect = width / height;
    camera.updateProjectionMatrix();
});

let loader = new STLLoader();
loader.load( './apex.stl', function ( geometry ) {
    console.log(geometry);
    let material = new THREE.MeshNormalMaterial( {
        color: 0x6c7a89,
    } );
    let mesh = new THREE.Mesh( geometry, material );
    scene.add( mesh );
    geometry.center();
    geometry.translate(1, 0, 0); //Account for bounding box center error
    camera.lookAt( mesh.position);
} );

const axesHelper = new THREE.AxesHelper( 100);
scene.add( axesHelper );

// Camera positioning
camera.position.x = 72;
camera.position.y = 25;
camera.position.z = 100;

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