function drawfunc() {

    //描画用の設定値を取得
    var dColor = drawst(draw[drawIndex], 3);
    var dWidth = drawst(draw[drawIndex], 4);
    var dType = drawst(draw[drawIndex], 5);
    var dPoint = "";
    var dPointarray = new Array();

    //タイプ別処理
    switch (dType) {

        case "line":
            //座標文字列の取得
            //座標は(始点X,終点X,始点Y,終点Y)
            dPoint = draw[drawIndex].substring(draw[drawIndex].search(dType) + 5);
            dPointarray = zerocut(dPoint.split(","));
            drawline(dPointarray, dColor, dWidth);
            break;

        case "rect":
            dPoint = draw[drawIndex].substring(draw[drawIndex].search(dType) + 5);
            dPointarray = zerocut(dPoint.split(","));
            drawrect(dPointarray, dColor, dWidth);
            break;

        case "freehand":
            dPoint = draw[drawIndex].substring(draw[drawIndex].search(dType) + 9);
            dPointarray = repPointAll(zerocut(dPoint.split(",")));
            drawfreehand(dPointarray, dColor, dWidth);
            break;

        case "mark":
            dPoint = draw[drawIndex].substring(draw[drawIndex].search(dType) + 5);
            dPointarray = zerocut(dPoint.split(","));
            drawmark(dPointarray, dColor, dWidth);
            break;

        case "eraser":
            drawclear();
            break;
    }

    savePic();  //test
    drawIndex++;
}

//直線描画処理
function drawline(Pointarray, Color, Width) {

    var canvas = document.getElementById("can1");

    var context = canvas.getContext("2d");

    context.beginPath();

    //プロパティの設定
    context.lineCap = "round";
    context.lineWidth = Width/2;
    context.strokeStyle = "#" + Color;

    //始点終点の設定
    context.moveTo(repPointx(Pointarray[0]), repPointy(Pointarray[2]));
    context.lineTo(repPointx(Pointarray[1]), repPointy(Pointarray[3]));

    //ここで描画
    context.stroke();
}

//矩形描画処理
function drawrect(Pointarray, Color, Width) {

    var canvas = document.getElementById("can1");

    var context = canvas.getContext("2d");

    context.beginPath();

    //プロパティの設定
    context.lineCap = "round";
    context.lineWidth = Width/2;
    context.strokeStyle = "#" + Color;

    //始点終点の設定
    context.rect(repPointx(Pointarray[0]),
                 repPointy(Pointarray[2]),
                 repPointx(Pointarray[1]) - repPointx(Pointarray[0]),
                 repPointy(Pointarray[3]) - repPointy(Pointarray[2]));

    //ここで描画
    context.stroke();
}

//自由曲線描画処理
function drawfreehand(Pointarray, Color, Width) {

    var canvas = {
        0: document.getElementById("can1"),
        1: document.getElementById("can2")
    };

    var ga_flip = 0;

    var context = canvas[ga_flip].getContext('2d');
    var b_context = canvas[ga_flip+1].getContext('2d');

    //描画パスの初期化
    context.beginPath();
    b_context.beginPath();

    //プロパティの設定
    context.lineCap = "round";
    context.lineWidth = Width/2;
    context.strokeStyle = "#" + Color;
    b_context.lineCap = "round";
    b_context.lineWidth = Width/2;
    b_context.strokeStyle = "#" + Color;

    //配列移動を初期化
    freespan = 4;

    //描画タイマーセット
    timerdraw(Pointarray, context, b_context);
}
function timerdraw(dpoint, cd, b_cd) {

    cd.moveTo(dpoint[freespan - 3], dpoint[(freespan - 1)]);

    if (freespan+4 <= dpoint.length) {
        b_cd.moveTo(dpoint[freespan + 1], dpoint[(freespan + 3)]);
    }

    cd.lineTo(dpoint[freespan], dpoint[(freespan + 2)]);

    if (freespan + 4 <= dpoint.length) {
        b_cd.lineTo(dpoint[freespan + 4], dpoint[(freespan + 6)]);
    }

    //配列移動
    freespan = freespan + 4;

    //ここで描画
    cd.stroke();
    b_cd.stroke();

    //終了判定
    if (freespan >= dpoint.length) {
    } else {
        freetime = setTimeout(function () { timerdraw(dpoint, cd, b_cd); }, 5);
    }
}

//マーク描画処理
function drawmark(Pointarray, Color, markid) {

    var canvas = document.getElementById("can1");

    var context = canvas.getContext("2d");

    //画像の設定
    var imageObj = new Image();
    imageObj.src = getmarkstr(markid);
    /*var ih = (imageObj.height / 2);
    var iw = (imageObj.width / 2);
    var dh = repPointy(Pointarray[3]) - ih;
    var dw = repPointx(Pointarray[1]) - iw;*/
    imageObj.onload = function () {
        do {
            var ih = (imageObj.height / 2);
            var iw = (imageObj.width / 2);
        } while (ih == 0 && iw == 0);
        var dh = repPointy(Pointarray[3]) - ih;
        var dw = repPointx(Pointarray[1]) - iw;
        context.drawImage(imageObj, dw, dh);
    }
}

//canvas描画削除処理
function drawclear() {

    var canvas = {
        0: document.getElementById("can1"),
        1: document.getElementById("can2")
    };

    var ga_flip = 0;

    var context = canvas[ga_flip].getContext('2d');
    var b_context = canvas[ga_flip + 1].getContext('2d');

    context.clearRect(0, 0, wsizex, wsizey);
    b_context.clearRect(0, 0, wsizex, wsizey);

}

//先頭のゼロを消す処理
function zerocut(stringarray) {
    for (var u = 0; u < stringarray.length; u++) {
        while (stringarray[u].search("0") == 0) {
            stringarray[u] = stringarray[u].substring(1);
        }
    }
    return stringarray;
}

//パーセントをピクセル化
//X座標
function repPointx(p) {
    var q = ssizex * p / 100;
    return q;
    //return q.toFixed(0);
}
//Y座標
function repPointy(p) {
    var q = ssizey * p / 100;
    return q;
    //return q.toFixed(0);
}
//一気に
function repPointAll(aarray) {
    var m = aarray.length;
    for (var y = 0; y < m; y = y + 4) {
        aarray[y] = repPointx(aarray[y]);
        aarray[y+1] = repPointx(aarray[y+1]);
        aarray[y+2] = repPointy(aarray[y+2]);
        aarray[y+3] = repPointy(aarray[y+3]);
    }
    return aarray;
}

//zoom切り替え自にローカルストレージに保存
function savePic() {
    // Canvasからbase64エンコーディングされた画像データを取得する
    var canvas = document.getElementById("can1");
    var b_canvas = document.getElementById("can2");
    var b_can1 = canvas.toDataURL();
    var b_can2 = canvas.toDataURL();

    if (fWebstorage == 1) {
        // sessionStorageに保存する
        window.sessionStorage.setItem("savecan1", b_can1);
        window.sessionStorage.setItem("savecan2", b_can2);
    }
}

//zoomout自ページ切り替え前の場合はローカルストレージから戻す
function loadPic() {
    var canvas = document.getElementById("can1");
    var b_canvas = document.getElementById("can2");
    var context = canvas.getContext("2d");
    var b_context = b_canvas.getContext("2d");

    if (fWebstorage == 1) {
        // sessionStorageからデータを取得する
        var b_can1 = window.sessionStorage.getItem("savecan1");
        var b_can2 = window.sessionStorage.getItem("savecan2");

        // Imageオブジェクトを作成し、src属性にデータを設定する
        var image1 = new Image();
        var image2 = new Image();
        image1.src = b_can1;
        image2.src = b_can2;
        image1.onload = function () {

            // 画像の読み込みが終わったら、Canvasに画像を反映する。
            context.drawImage(image1, 0, 0);
            b_context.drawImage(image2, 0, 0);
        }
    }
}

//zoomin中ページ切り替えが発生した場合はセッションストレージから削除
function clearPic() {
    if (fWebstorage == 1) {
        window.sessionStorage.clear();
    }
}

//canvasサイズ切り替えした際にセッションストレージから戻す
function resizePic() {
    loadPic();
}