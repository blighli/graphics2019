// 地图
var map = [ // 1  2  3  4  5  6  7  8  9
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1,], // 0
    [1, 1, 0, 0, 0, 0, 0, 1, 1, 1,], // 1
    [1, 1, 0, 0, 2, 0, 2, 0, 0, 1,], // 2
    [1, 0, 0, 0, 0, 0, 0, 0, 1, 1,], // 3
    [1, 0, 0, 2, 0, 0, 2, 0, 0, 1,], // 4
    [1, 0, 0, 2, 2, 0, 0, 0, 1, 1,], // 5
    [1, 1, 0, 0, 0, 0, 0, 1, 1, 1,], // 6
    [1, 1, 0, 0, 0, 1, 0, 0, 1, 1,], // 7
    [1, 1, 1, 1, 1, 1, 0, 0, 1, 1,], // 8
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1,], // 9
    ], mapW = map.length, mapH = map[0].length;

// 可以调整的参数
var WIDTH = window.innerWidth,
HEIGHT = window.innerHeight,
ASPECT = WIDTH / HEIGHT,
UNITSIZE = 250,
WALLHEIGHT = UNITSIZE / 3,
MOVESPEED = 100,
LOOKSPEED = 0.075,
BULLETMOVESPEED = MOVESPEED * 5,
NUMAI = 5,
PROJECTILEDAMAGE = 20;

// 全局变量
var t = THREE, scene, cam, renderer, controls, clock, projector, model, skin;
var runAnim = true, mouse = { x: 0, y: 0 }, kills = 0, health = 100;
var healthCube, lastHealthPickup = 0;


// 初始化
$(document).ready(function() {
    $('body').append('<div id="intro">Click to start playing the game!</div>');
    $('#intro').css({width: WIDTH, height: HEIGHT}).one('click', function(e) {
        e.preventDefault();
        $(this).fadeOut();
        init();
        setInterval(drawRadar, 1000);
        animate();
    });
});

// 设置
function init() {
    clock = new t.Clock(); // 在 render() 中使用，为了 controls.update()
    projector = new t.Projector(); // 用于子弹发射
    scene = new t.Scene(); // 场景
    scene.fog = new t.FogExp2(0xD6F1FF, 0.0005); // 雾

    // 设置相机
    cam = new t.PerspectiveCamera(60, ASPECT, 1, 10000); 
    /* 
    fov — Camera frustum vertical field of view.
    aspect — Camera frustum aspect ratio.
    near — Camera frustum near plane.
    far — Camera frustum far plane.
    */
    cam.position.y = UNITSIZE * .2;
    scene.add(cam);

    // 第一人称视角，使用 WASD 移动
    controls = new t.FirstPersonControls(cam);
    controls.movementSpeed = MOVESPEED;
    controls.lookSpeed = LOOKSPEED;
    controls.lookVertical = false; // 无法上下看
    controls.noFly = true;

    // 世界中的场景
    setupScene();

    // 敌人 AI
    setupAI();

    //  使用 WebGL 渲染
    renderer = new t.WebGLRenderer();
    renderer.setSize(WIDTH, HEIGHT);

    // 加入 document
    renderer.domElement.style.backgroundColor = '#D6F1FF'; // easier to see
    document.body.appendChild(renderer.domElement);

    //  追踪鼠标的位置
    document.addEventListener( 'mousemove', onDocumentMouseMove, false );

    //  点击进行射击
    $(document).click(function(e) {
        e.preventDefault;
        if (e.which === 1) { // 左键单击
            createBullet();
        }
    });

    // 游戏显示仪
    $('body').append('<canvas id="radar" width="200" height="200"></canvas>');
    $('body').append('<div id="hud"><p>HP: <span id="health">100</span><br />Score: <span id="score">0</span></p></div>');
    $('body').append('<div id="credits"><p>WASD or arrow key to move <br/> use the mouse to change the perspective <br> left-click to fire</p></div>');

    // 受伤警报
    $('body').append('<div id="hurt"></div>');
    $('#hurt').css({width: WIDTH, height: HEIGHT,});
}

// 不断刷新帧
function animate() {
    if (runAnim) {
        requestAnimationFrame(animate);
    }
    render();
}

// 更新
function render() {
    var delta = clock.getDelta(), speed = delta * BULLETMOVESPEED;
    var aispeed = delta * MOVESPEED;
    controls.update(delta); // 移动相机

    // 旋转急救包
    healthcube.rotation.x += 0.004
    healthcube.rotation.y += 0.008;
    // 每分钟只能取一次
    if (Date.now() > lastHealthPickup + 60000) {
        if (distance(cam.position.x, cam.position.z, healthcube.position.x, healthcube.position.z) < 15 && health != 100) {
            health = Math.min(health + 50, 100);
            $('#health').html(health);
            lastHealthPickup = Date.now();
        }
        healthcube.material.wireframe = false;
    }
    else {
        healthcube.material.wireframe = true;
    }

    // 更新子弹，从末尾删除，保证复杂度
    for (var i = bullets.length-1; i >= 0; i--) {
        var b = bullets[i], p = b.position, d = b.ray.direction;
        if (checkWallCollision(p)) {
            bullets.splice(i, 1); // 删除 i 位置的子弹
            scene.remove(b);
            continue;
        }
        // 子弹与 AI 敌人碰撞
        var hit = false;
        for (var j = ai.length-1; j >= 0; j--) {
            var a = ai[j];
            // Begin Geometry.vertices is undefined
            //  var temp_vertices = a.children[0].geometry.position.array;

            //  var temp_a_geometry = new THREE.Geometry().fromBufferGeometry(a['geometry']);
            //  var v = temp_vertices[0];
            //  console.log(v)
            // End Geometry.vertices is undefined
            var v = a.geometry.vertices[0];
            var c = a.position;
            var x = Math.abs(v.x), z = Math.abs(v.z);
            if (p.x < c.x + x && p.x > c.x - x &&
                    p.z < c.z + z && p.z > c.z - z &&
                    b.owner != a) {
                bullets.splice(i, 1);
                scene.remove(b);
                a.health -= PROJECTILEDAMAGE;
                var color = a.material.color, percent = a.health / 100;
                a.material.color.setRGB(
                        percent * color.r,
                        percent * color.g,
                        percent * color.b
                );
                hit = true;
                break;
            }
        }
        // 自己被子弹击中
        if (distance(p.x, p.z, cam.position.x, cam.position.z) < 25 && b.owner != cam) {
            $('#hurt').fadeIn(75);
            health -= 10;
            if (health < 0) health = 0;
            val = health < 25 ? '<span style="color: darkRed">' + health + '</span>' : health;
            $('#health').html(val);
            bullets.splice(i, 1);
            scene.remove(b);
            $('#hurt').fadeOut(350);
        }

        if (!hit) {
            b.translateX(speed * d.x);
            b.translateZ(speed * d.z);
        }
    }

    // 更新 AI 敌人
    for (var i = ai.length-1; i >= 0; i--) {
        var a = ai[i];
        if (a.health <= 0) {
            ai.splice(i, 1);
            scene.remove(a);
            kills++;
            $('#score').html(kills * 100);
        }

        // AI 敌人移动
        
        var r = Math.random();
        if (r > 0.995) {
            a.lastRandomX = Math.random() * 2 - 1;
            a.lastRandomZ = Math.random() * 2 - 1;
        }
        // ADD BEGIN
        //a.lastRandomX = 0;
        //a.lastRandomZ = 0;
        // ADD END

        a.translateX(aispeed * a.lastRandomX);
        a.translateZ(aispeed * a.lastRandomZ);

        var c = getMapSector(a.position);
        if (c.x < 0 || c.x >= mapW || c.y < 0 || c.y >= mapH || checkWallCollision(a.position)) {
            a.translateX(-2 * aispeed * a.lastRandomX);
            a.translateZ(-2 * aispeed * a.lastRandomZ);
            a.lastRandomX = Math.random() * 2 - 1;
            a.lastRandomZ = Math.random() * 2 - 1;
        }
        if (c.x < -1 || c.x > mapW || c.z < -1 || c.z > mapH) {
            ai.splice(i, 1);
            scene.remove(a);
            addAI();
        }
 
        var cc = getMapSector(cam.position);
        if (Date.now() > a.lastShot + 750 && distance(c.x, c.z, cc.x, cc.z) < 2) {
            createBullet(a);
            a.lastShot = Date.now();
        }
    }

    renderer.render(scene, cam); // 重绘

    // 死亡
    if (health <= 0) {
        runAnim = false;
        $(renderer.domElement).fadeOut();
        $('#radar, #hud, #credits').fadeOut();
        $('#intro').fadeIn();
        $('#intro').html('Unfortunately, you are dead! Click to restart the game!');
        $('#intro').one('click', function() {
            location = location;
        });
    }

    if (kills >= NUMAI){
        runAnim = false;
        $(renderer.domElement).fadeOut();
        $('#radar, #hud, #credits').fadeOut();
        $('#intro').fadeIn();
        $('#intro').html('Congratulations! You won!');
        $('#intro').one('click', function() {
            location = location;
        });
    }
}

// 设置世界中的物体
function setupScene() {
    var UNITSIZE = 250, units = mapW;

    // 地面
    var floor = new t.Mesh(
        new t.CubeGeometry(units * UNITSIZE, 10, units * UNITSIZE),
        new t.MeshLambertMaterial({map: new t.TextureLoader().load('images/soil-image2.jpg')})
    );
    scene.add(floor);

    // 墙
    var cube = new t.CubeGeometry(UNITSIZE, WALLHEIGHT, UNITSIZE);
    var materials = [
        new t.MeshLambertMaterial({map: new t.TextureLoader().load('images/wall-3.jpg')}),
        new t.MeshLambertMaterial({map: new t.TextureLoader().load('images/wall-3.jpg')}),
        new t.MeshLambertMaterial({color: 0xFBEBCD}),
    ];
    for (var i = 0; i < mapW; i++) {
        for (var j = 0, m = map[i].length; j < m; j++) {
            if (map[i][j]) {
                var wall = new t.Mesh(cube, materials[map[i][j]-1]);
                wall.position.x = (i - units/2) * UNITSIZE;
                wall.position.y = WALLHEIGHT/2;
                wall.position.z = (j - units/2) * UNITSIZE;
                scene.add(wall);
            }
        }
    }

    // 急救包
    healthcube = new t.Mesh(
        new t.CubeGeometry(30, 30, 30),
        new t.MeshBasicMaterial({map: new t.TextureLoader().load('images/red-cross.png')})
    );
    healthcube.position.set(-UNITSIZE-15, 35, -UNITSIZE-15);
    scene.add(healthcube);

    // 光照
    var directionalLight1 = new t.DirectionalLight( 0xF7EFBE, 0.7 );
    directionalLight1.position.set( 0.5, 1, 0.5 );
    scene.add( directionalLight1 );
    var directionalLight2 = new t.DirectionalLight( 0xF7EFBE, 0.5 );
    directionalLight2.position.set( -0.5, -1, -0.5 );
    scene.add( directionalLight2 );
}

var ai = [];
var aiGeo = new t.CubeGeometry(40, 40, 40);
function setupAI() {
    for (var i = 0; i < NUMAI; i++) {
        addAI();
    }
}

function addAI() {
    var c = getMapSector(cam.position);
    var aiMaterial = new t.MeshBasicMaterial({map: new t.TextureLoader().load('images/toxic.png')});
    
    var o = new t.Mesh(aiGeo, aiMaterial);

    // Begin Add Tank
    // var o = new THREE.Mesh();
    /*
    var loader = new THREE.OBJLoader();
    loader.load("../models/male02.obj", function(o){
        // o.traverse(function(child){
        //     if(child instanceof THREE.Mesh){
        //         child.material.map = aiMaterial;
        //     }
        // })
        o.scale.set(0.1, 0.1, 0.1);

        do {
            var x = getRandBetween(0, mapW-1);
            var z = getRandBetween(0, mapH-1);
        } while (map[x][z] > 0 || (x == c.x && z == c.z));
        x = Math.floor(x - mapW/2) * UNITSIZE;
        z = Math.floor(z - mapW/2) * UNITSIZE;
        o.position.set(x, UNITSIZE * 0.15, z);
        o.health = 100;
        //o.path = getAIpath(o);
        o.pathPos = 1;
        o.lastRandomX = Math.random();
        o.lastRandomZ = Math.random();
        o.lastShot = Date.now(); // Higher-fidelity timers aren't a big deal here.
        
        ai.push(o);

        scene.add(o);

    });
    */
    // End Add Tank
    
    do {
        var x = getRandBetween(0, mapW-1);
        var z = getRandBetween(0, mapH-1);
    } while (map[x][z] > 0 || (x == c.x && z == c.z));
    x = Math.floor(x - mapW/2) * UNITSIZE;
    z = Math.floor(z - mapW/2) * UNITSIZE;
    o.position.set(x, UNITSIZE * 0.15, z);
    o.health = 100;
    o.pathPos = 1;
    o.lastRandomX = Math.random();
    o.lastRandomZ = Math.random();
    o.lastShot = Date.now(); 
    ai.push(o);
    scene.add(o);
}

function distance(x1, y1, x2, y2) {
    return Math.sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

function getMapSector(v) {
    var x = Math.floor((v.x + UNITSIZE / 2) / UNITSIZE + mapW/2);
    var z = Math.floor((v.z + UNITSIZE / 2) / UNITSIZE + mapW/2);
    return {x: x, z: z};
}

/**
* Check whether a Vector3 overlaps with a wall.
*
* @param v
*   A THREE.Vector3 object representing a point in space.
*   Passing cam.position is especially useful.
* @returns {Boolean}
*   true if the vector is inside a wall; false otherwise.
*/
function checkWallCollision(v) {
    var c = getMapSector(v);
    return map[c.x][c.z] > 0;
}

// Radar
function drawRadar() {
    var c = getMapSector(cam.position), context = document.getElementById('radar').getContext('2d');
    context.font = '10px Helvetica';
    for (var i = 0; i < mapW; i++) {
        for (var j = 0, m = map[i].length; j < m; j++) {
            var d = 0;
            for (var k = 0, n = ai.length; k < n; k++) {
                var e = getMapSector(ai[k].position);
                if (i == e.x && j == e.z) {
                    d++;
                }
            }
            if (i == c.x && j == c.z && d == 0) {
                context.fillStyle = '#0000FF';
                context.fillRect(i * 20, j * 20, (i+1)*20, (j+1)*20);
            }
            else if (i == c.x && j == c.z) {
                context.fillStyle = '#AA33FF';
                context.fillRect(i * 20, j * 20, (i+1)*20, (j+1)*20);
                context.fillStyle = '#000000';
                context.fillText(''+d, i*20+8, j*20+12);
            }
            else if (d > 0 && d < 10) {
                context.fillStyle = '#FF0000';
                context.fillRect(i * 20, j * 20, (i+1)*20, (j+1)*20);
                context.fillStyle = '#000000';
                context.fillText(''+d, i*20+8, j*20+12);
            }
            else if (map[i][j] > 0) {
                context.fillStyle = '#666666';
                context.fillRect(i * 20, j * 20, (i+1)*20, (j+1)*20);
            }
            else {
                context.fillStyle = '#CCCCCC';
                context.fillRect(i * 20, j * 20, (i+1)*20, (j+1)*20);
            }
        }
    }
}

var bullets = [];
var sphereMaterial = new t.MeshBasicMaterial({color: 0x333333});
var sphereGeo = new t.SphereGeometry(2, 6, 6);
function createBullet(obj) {
    if (obj === undefined) {
        obj = cam;
    }
    var sphere = new t.Mesh(sphereGeo, sphereMaterial);
    sphere.position.set(obj.position.x, obj.position.y * 0.8, obj.position.z);

    if (obj instanceof t.Camera) {
        var vector = new t.Vector3(mouse.x, mouse.y, 1);
        vector.unproject(obj);
        sphere.ray = new t.Ray(
                obj.position,
                vector.sub(obj.position).normalize()
        );
    }
    else {
        var vector = cam.position.clone();
        sphere.ray = new t.Ray(
                obj.position,
                vector.sub(obj.position).normalize()
        );
    }
    sphere.owner = obj;

    bullets.push(sphere);
    scene.add(sphere);

    return sphere;
}

function onDocumentMouseMove(e) {
    e.preventDefault();
    mouse.x = (e.clientX / WIDTH) * 2 - 1;
    mouse.y = - (e.clientY / HEIGHT) * 2 + 1;
}

// 窗口大小缩放
$(window).resize(function() {
    WIDTH = window.innerWidth;
    HEIGHT = window.innerHeight;
    ASPECT = WIDTH / HEIGHT;
    if (cam) {
        cam.aspect = ASPECT;
        cam.updateProjectionMatrix();
    }
    if (renderer) {
        renderer.setSize(WIDTH, HEIGHT);
    }
    $('#intro, #hurt').css({width: WIDTH, height: HEIGHT,});
});

// 当聚焦点不在视窗时，停止移动
$(window).focus(function() {
    if (controls) controls.freeze = false;
});
$(window).blur(function() {
    if (controls) controls.freeze = true;
});

// 得到 [lo, hi] 之间的随机数
function getRandBetween(lo, hi) {
    return parseInt(Math.floor(Math.random()*(hi-lo+1))+lo, 10);
}
