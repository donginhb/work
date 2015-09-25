
var v = videojs("video");

//動画データ再生開始時
function playVideo() {
    v.play();
    fvseek = 0;
}

function poscheck() {

    if (!fvseek)
        sPos();
}

v.on("seeked", function () {
    fvseek = 0;
}, false);

v.on("seeking", function () {
    fvseek = 1;
}, false);

v.on("ended", function () {
    if (fScorm) {
        doScore(100);
        doQuit();
    }
    fvend = 1;
}, false);

v.on("play", function () {
    if (fvend)
        preparam();
}, false);

function timestart() {
    v.on("timeupdate", poscheck, false);
}

timestart();

function pauseVideo() {
    //動画を一時停止
    v.pause();
}

//動画データ読み込み完了時
//モバイル機器では不発
v.on("loadeddata", function () {
    f_comp = 1;
    v.play();
}, false);


/////////////////////////////////////////別のJSファイルへ移動予定

//demo表示
function demostart(demofile) {
    if (!bStreaming) {
        objectall();
    }

    var dsrc = 'movie/' + demofile + '.mp4';

    var demov = videojs("demo");
    var demoa = document.getElementById("demoarea");

    demov.src(dsrc);

    demoa.style.display = "block";

    demov.width(wsizex - 15);
    demov.height(wsizey - 45);

    demov.play();

    //再生完了を知らせる
    demov.on("ended", function () {
        demoa.style.display = "none";
        var bln = mMark - 1;
        var ln = gScriptTable[bln].mLayno;
        demov.currentTime(0);
        demov.pause();
        ChangeLayout(ln);
        if (fZoom)
            ChangeLayout("999");
        playVideo();
    }, false);
}

//デモ再生用関数
function demoplay() {
    var demov = videojs("demo");
    demov.play();
}

//ローディング画像非表示処理
function lpicv(obj) {
    obj.style.display = "none";
}