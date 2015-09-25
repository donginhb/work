//フラグによる表示処理
function ImageExist(url) {
    var img = new Image();
    img.src = url;
    return img.height != 0;
}
//背景画像表示
if (bBackimage) {
    if(ImageExist("_backgnd.jpg")){
        window.document.body.background = "_backgnd.jpg";
    }else{
        window.document.body.background = "_backgnd.gif";
    }
} else {
    window.document.body.bgColor = bBackcolor;
}


//各ユーザーエージェントによってデザイン変更が必要かも
//デモポイント処理の追加
//ズームインズームアウト処理の追加
//SCORM処理を考慮

importJS();

// iOSの場合とそれ以外とで画面回転時を判定するイベントを切り替える
var event = navigator.userAgent.match(/(iPhone|iPod|iPad)/) ? 'orientationchange' : 'resize';
window.addEventListener(event, resize, false);

//リサイズ処理追加
function resize() {
    wsizex = window.innerWidth;
    wsizey = window.innerHeight;
    ChangeLayout(dLayout);
    if (fZoom)
        ChangeLayout("999");

    var demov = document.getElementById("demo");
    demov.width = wsizex;
    demov.height = wsizey - 5;

    if (drawflag == 1)
        resizePic();
}

//拡大率算出関数 戻りは拡大率
function zoomp() {
    var qx = 0;
    var qy = 0;
    var s = document.getElementById("sframe");

    qx = s.clientWidth / ssizex;
    qy = s.clientHeight / ssizey;

    if (qx >= 1 && qy >= 1) {
        if (qx <= qy) {
            return qx;
        } else {
            return qy;
        }
    } else if (qx < 1 && qy < 1) {
        if (qx >= qy) {
            return qy;
        } else {
            return qx;
        }
    } else {
        if (qx >= qy) {
            return qy;
        } else {
            return qx;
        }
    }
}

//格納フォルダの変更
function importJS() {
    document.writeln('<script type="text/javascript" src="layout001.js"></script>');
    document.writeln('<script type="text/javascript" src="layout002.js"></script>');
    document.writeln('<script type="text/javascript" src="layout003.js"></script>');
    document.writeln('<script type="text/javascript" src="layout004.js"></script>');
    document.writeln('<script type="text/javascript" src="layout005.js"></script>');
    document.writeln('<script type="text/javascript" src="layout006.js"></script>');
    document.writeln('<script type="text/javascript" src="layout007.js"></script>');
    document.writeln('<script type="text/javascript" src="layout008.js"></script>');
    document.writeln('<script type="text/javascript" src="layout009.js"></script>');
    document.writeln('<script type="text/javascript" src="layout010.js"></script>');
    document.writeln('<script type="text/javascript" src="layout011.js"></script>');
    document.writeln('<script type="text/javascript" src="layout012.js"></script>');
    document.writeln('<script type="text/javascript" src="layout013.js"></script>');
    document.writeln('<script type="text/javascript" src="layout014.js"></script>');
    document.writeln('<script type="text/javascript" src="layout015.js"></script>');
    document.writeln('<script type="text/javascript" src="layout016.js"></script>');
    document.writeln('<script type="text/javascript" src="layout017.js"></script>');
    document.writeln('<script type="text/javascript" src="layout018.js"></script>');
    document.writeln('<script type="text/javascript" src="layout019.js"></script>');
    document.writeln('<script type="text/javascript" src="layout020.js"></script>');
    document.writeln('<script type="text/javascript" src="layout021.js"></script>');
    document.writeln('<script type="text/javascript" src="layout022.js"></script>');
    document.writeln('<script type="text/javascript" src="layout023.js"></script>');
    document.writeln('<script type="text/javascript" src="layout999.js"></script>');
}

function ChangeLayout(layNo) {

    switch (layNo) {
        case "001":
            var l = new lay001();
            break;

        case "002":
            var l = new lay002();
            break;

        case "003":
            var l = new lay003();
            break;

        case "004":
            var l = new lay004();
            break;

        case "005":
            var l = new lay005();
            break;

        case "006":
            var l = new lay006();
            break;

        case "007":
            var l = new lay007();
            break;

        case "008":
            var l = new lay008();
            break;

        case "009":
            var l = new lay009();
            break;

        case "010":
            var l = new lay010();
            break;

        case "011":
            var l = new lay011();
            break;

        case "012":
            var l = new lay012();
            break;

        case "013":
            var l = new lay013();
            break;

        case "014":
            var l = new lay014();
            break;

        case "015":
            var l = new lay015();
            break;

        case "016":
            var l = new lay016();
            break;

        case "017":
            var l = new lay017();
            break;

        case "018":
            var l = new lay018();
            break;

        case "019":
            var l = new lay019();
            break;

        case "020":
            var l = new lay020();
            break;

        case "021":
            var l = new lay021();
            break;

        case "022":
            var l = new lay022();
            break;

        case "023":
            var l = new lay023();
            break;

        case "999":
            var l = new lay999();
            break;
    }

    layoutNo = layNo;

    l.setVarea();
    l.setTiarea();
    l.setLarea();
    l.setInarea();
    l.setSarea();
    l.setIVarea();
    l.setMarea();
    l.setCanarea();
}

//全オブジェクト表示/非表示
function objectall() {
    var obj1 = document.getElementById("indexarea");
    var obj2 = document.getElementById("logoarea");
    var obj3 = document.getElementById("slidearea");
    var obj4 = document.getElementById("videoarea");
    var obj6 = document.getElementById("demoarea");
    var obj7 = document.getElementById("titlearea");

    obj1.style.display = "none";
    obj2.style.display = "none";
    obj3.style.display = "none";
    obj4.style.display = "none";
    obj6.style.display = "none";
    obj7.style.display = "none";
}