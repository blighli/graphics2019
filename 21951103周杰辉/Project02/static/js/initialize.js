(function () {
    var global = this;
    
    var THREE = global.THREE;
    var requestAnimationFrame = global.requestAnimationFrame;

    var renderer, scene, camera, controls, light, material, raycaster;
    var mouse;
    var tooltipEnabledObjects;// collect objects for raycasting
    // for better performance don't raytrace all scene
    var latestMouseProjection; 
    // this is the latest projection of the mouse on object (i.e. intersection with ray)
    var hoveredObj; // this objects is hovered at the moment
    var tooltipDisplayTimeout;
    // tooltip will not appear immediately. If object was hovered shortly,
    // - the timer will be canceled and tooltip will not appear at all.
    var theMilkyWay, theSun, earth, moon;
    
    var AU = 50; // astronomical unit

    var milkyWaySize = 15000;

    var sunSize = 10.15;
    
    var earthSize = 5,
        earthOrbitRadius = sunSize + AU,
        earthOrbitAngle = getRandomArbitrary(0, 360),
        earthOrbitSpeed = 0.2,
        earthRotateSpeed = 0.1;

    var moonSize = 2,
        moonOrbitRadius = (AU * 0.2),
        moonOrbitAngle = getRandomArbitrary(0, 360),
        moonOrbitSpeed = 1.5,
        moonRotateSpeed = 0.01;
    
    var WIDTH = window.innerWidth,
        HEIGHT = window.innerHeight;

    init();
    animate();

    function init() {
        scene = new THREE.Scene();
        scene.fog = new THREE.FogExp2(0x000000, 0.00008);

        raycaster = new THREE.Raycaster();

        camera = new THREE.PerspectiveCamera(70, window.innerWidth / window.innerHeight, 1, 100000);
        camera.position.z = 150;

        controls = new THREE.TrackballControls(camera);
        controls.rotateSpeed = 1.0;
        controls.zoomSpeed = 1.2;
        controls.panSpeed = 0.8;
        controls.noZoom = false;
        controls.noPan = false;
        controls.staticMoving = true;
        controls.dynamicDampingFactor = 0.3;

        renderer = new THREE.WebGLRenderer();
        renderer.setSize(WIDTH, HEIGHT);
        document.body.appendChild(renderer.domElement);
    
        //sunlight
        light = new THREE.PointLight(0xffffff, 1, 0);
        light.position.set(0, 0, 0);
        scene.add(light);
    
        tooltipEnabledObjects = [];

        //the Milky Way
        material = new THREE.MeshBasicMaterial({
            map: THREE.ImageUtils.loadTexture('static/img/milky-way.jpg'),
            side: THREE.DoubleSide
        });
        theMilkyWay = new THREE.Mesh(new THREE.SphereGeometry(milkyWaySize, 35, 35 ), material);
        scene.add(theMilkyWay);
    
    
        //the sun
        //SphereGeometry(size of sphere, higher the numeber the smoother the sphere and more processing)
        material = new THREE.MeshBasicMaterial({
            map: THREE.ImageUtils.loadTexture('static/img/sun.jpg'),
            side: THREE.DoubleSide
        });
        theSun = new THREE.Mesh(new THREE.SphereGeometry(sunSize, 35, 35 ), material);
        theSun.userData.tooltipText = "Sun";
        scene.add(theSun);
        tooltipEnabledObjects.push(theSun);
    
        //earth
        material = new THREE.MeshLambertMaterial({
            map: THREE.ImageUtils.loadTexture('static/img/earth.jpg'),
            shading: THREE.SmoothShading
        });
        earth = new THREE.Mesh(new THREE.SphereGeometry(earthSize, 15, 15), material);
        earth.userData.tooltipText = "Earth";
        scene.add(earth);
        tooltipEnabledObjects.push(earth);

        //moon
        material = new THREE.MeshLambertMaterial({
            map: THREE.ImageUtils.loadTexture('static/img/moon.jpg'),
            shading: THREE.SmoothShading
        });
        moon = new THREE.Mesh(new THREE.SphereGeometry(moonSize, 15, 15), material);
        moon.userData.tooltipText = "Moon";
        scene.add(moon);
        tooltipEnabledObjects.push(moon);

        mouse = new THREE.Vector2();

        window.addEventListener('resize', onWindowResize, false);
        window.addEventListener('mousemove', onMouseMove, false);
    }

    function animate() {
        requestAnimationFrame(animate);
        render();
    }

    function render() {
        controls.update();
        
        theSun.rotation.y += 0.001;
        
        //run earth's orbit around the Sun
        earthOrbitAngle -= earthOrbitSpeed;
        var radians = earthOrbitAngle * Math.PI / 180;
        earth.position.x = Math.cos(radians) * earthOrbitRadius;
        earth.position.z = Math.sin(radians) * earthOrbitRadius;
        earth.rotation.y += earthRotateSpeed;

        //run moon's orbit around the earth
        moonOrbitAngle -= moonOrbitSpeed;
        var radians = moonOrbitAngle * Math.PI / 180;
        moon.position.x = earth.position.x + Math.cos(radians) * moonOrbitRadius;
        moon.position.z = earth.position.z + Math.sin(radians) * moonOrbitRadius;
        moon.rotation.y += moonRotateSpeed;
        
        renderer.render( scene, camera );
    }

    function onWindowResize() {
        camera.aspect = window.innerWidth / window.innerWidth;
        camera.updateProjectionMatrix();

        renderer.setSize( window.innerWidth, window.innerWidth );
    }

    // This will move tooltip to the current mouse position and show it by timer.
    function showTooltip(){
        var divElement = $("#tooltip");

        if(divElement && latestMouseProjection){
            divElement.css({
                display: "block",
                opacity: 0.0
            });

            var canvasHalfWidth = renderer.domElement.offsetWidth / 2;
            var canvasHalfHeight = renderer.domElement.offsetHeight / 2;

            var tooltipPosition = latestMouseProjection.clone().project(camera);
            tooltipPosition.x = (tooltipPosition.x * canvasHalfWidth) + canvasHalfWidth + renderer.domElement.offsetLeft;
            tooltipPosition.y = -(tooltipPosition.y * canvasHalfHeight) + canvasHalfHeight + renderer.domElement.offsetTop;

            var tootipWidth = divElement[0].offsetWidth;
            var tootipHeight = divElement[0].offsetHeight;

            divElement.css({
                left: `${tooltipPosition.x - tootipWidth/2}px`,
                top:  `${tooltipPosition.y - tootipHeight - 5}px`
            });

            divElement.text(hoveredObj.userData.tooltipText);

            setTimeout(function(){
                divElement.css({
                    opacity: 1.0
                });
            }, 25);
        }
    }

    // This will immediately hide tooltip.
    function hideTooltip(){
        // var divElement = document.getElementById("tooltip");
        var divElement = $("#tooltip");
        if(divElement){
            divElement.css({
                display: "none"
            });
        }
    }

    // Following two functions will convert mouse coordinates
    // from screen to three.js system (where [0,0] is in the middle of the screen)
    function updateMouseCoords(event, coordsObj){
        coordsObj.x = ((event.clientX - renderer.domElement.offsetLeft + 0.5) / window.innerWidth) * 2 - 1;
        coordsObj.y = -((event.clientY - renderer.domElement.offsetTop + 0.5) / window.innerHeight) * 2 + 1;
    }

    function handleManipulationUpdate(){
        raycaster.setFromCamera(mouse, camera);
        var intersects = raycaster.intersectObjects(tooltipEnabledObjects);
        if (intersects.length > 0) {
            latestMouseProjection = intersects[0].point;
            hoveredObj = intersects[0].object;
        }
        if(tooltipDisplayTimeout || !latestMouseProjection){
            clearTimeout(tooltipDisplayTimeout);
            tooltipDisplayTimeout = undefined;
            hideTooltip();
        }

        if(!tooltipDisplayTimeout && latestMouseProjection){
            tooltipDisplayTimeout = setTimeout(function(){
                tooltipDisplayTimeout = undefined;
                showTooltip();
            }, 25);
        }
    }

    function onMouseMove(event){
        updateMouseCoords(event, mouse);
        latestMouseProjection = undefined;
        hoveredObj = undefined;
        handleManipulationUpdate(); 
    }

    function getRandomArbitrary(min, max) {
        return Math.random() * (max - min) + min;
    }
    
}).call(this);