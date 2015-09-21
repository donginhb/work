//コンテンツタイトルに使用するフォント
var TitleFont = "\"Hiragino Kaku Gothic ProN\",\"メイリオ\", sans-serif";
//コンテンツタイトル表示のフォントサイズ
var TitleSize = "24px";

//配色設定
switch (bBpattern) {
    case 1: //標準(クリーム色)
        //コンテンツタイトル表示のフォントカラー
        var Titlecolor = "#000000";
        //コンテンツ背景色パターン
        var bBackcolor = "#ffffc0";
        //インデックスタブの色指定
        var bTabcolor1 = "Red";
        //メッセージタブの色指定
        var bTabcolor2 = "Blue";
        break;
    case 2: //白
        var Titlecolor = "#000000";
        var bBackcolor = "#FFFFFF";
        var bTabcolor1 = "Red";
        var bTabcolor2 = "Blue";
        break;
    case 3: //青
        var Titlecolor = "#FFFFFF";
        var bBackcolor = "blue";
        var bTabcolor1 = "#8080ff";
        var bTabcolor2 = "Green";
        break;
    case 4: //黒
        var Titlecolor = "#FFFFFF";
        var bBackcolor = "#000000";
        var bTabcolor1 = "#8080ff";
        var bTabcolor2 = "Green";
        break;
}

//各専用固定値を集約

var topx = 10;
var topy = 10;
var wsizex = window.innerWidth;
var wsizey = window.innerHeight;
var bNo = 0;
var fZoom = 0;
var desttop = 0;
var destleft = 0;
var destwidth = 0;
var destheight = 0;
var dLayout = layoutNo;
var szoomp = 1;
var drawIndex = 0;
var oldpoint1 = 0;
var oldpoint2 = 0;
var oldpoint3 = 0;
var oldpoint4 = 0;
var freespan = 0;
var freetime;
var mSnam = 1;
var mMark = 1;
var fvend = 0;
var fvseek = 0;
var arid = 1;

function preparam() {
    topx = 10;
    topy = 10;
    wsizex = window.innerWidth;
    wsizey = window.innerHeight;
    bNo = 0;
    fZoom = 0;
    desttop = 0;
    destleft = 0;
    destwidth = 0;
    destheight = 0;
    dLayout = layoutNo;
    szoomp = 1;
    drawIndex = 0;
    oldpoint1 = 0;
    oldpoint2 = 0;
    oldpoint3 = 0;
    oldpoint4 = 0;
    freespan = 0;
    //freetime;
    mSnam = 1;
    mMark = 1;
    fvend = 0;
    arid = 1;
}

//ブラウザ判断
function brow() {

var userAgent = window.navigator.userAgent.toLowerCase();
var appVersion = window.navigator.appVersion.toLowerCase();

if (userAgent.indexOf('opera') != -1) {
        return 'opera';
    } else if (userAgent.indexOf('opr') != -1) {
        return 'opera';
    } else if (userAgent.indexOf('android') != -1) {
        return 'Android';
    } else if (userAgent.indexOf('msie') != -1) {
        if (appVersion.indexOf("msie 6.") != -1) {
            return 'ie6';
        } else if (appVersion.indexOf("msie 7.") != -1) {
            return 'ie7';
        } else if (appVersion.indexOf("msie 8.") != -1) {
            return 'ie8';
        } else {
            return 'ie';
        }
    } else if (userAgent.indexOf('firefox') != -1) {
        return 'firefox';
    } else if (userAgent.indexOf('chrome') != -1) {
        return 'chrome';
    } else if (userAgent.indexOf('safari') != -1) {
        if (userAgent.indexOf('ipad') != -1) {
           return 'ipad';
        } else if (userAgent.indexOf('iphone') != -1) {
           return 'iphone';
        } else {
            return 'safari';
        }
    } else if (userAgent.indexOf('gecko') != -1) {
        return 'gecko';
    } else {
        return false;
    }
}

if (drawflag == 1) {
    //手書き情報を整理
    var drawtime = new Array();

    for (var u = 0; u < draw.length; u++) {
        drawtime[u] = drawst(draw[u], 2);
    }
}

//手書き開始時間取得
function drawst(dstr, i) {
    var oldIndex = 0;
    var nextIndex = 0;
    for (var u = 0; u < i; u++) {
        nextIndex = dstr.indexOf(",", oldIndex + 1);
        rstr = dstr.substring(oldIndex + 1, nextIndex);
        oldIndex = nextIndex;
    }
    return rstr;
}