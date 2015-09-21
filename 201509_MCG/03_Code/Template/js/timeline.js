function hScriptList(no, time, sno, ano, command,
                    indexname, layno, pro) {
    this.mNo = no;
    this.mTime = time;
    this.mSno = sno;
    this.mAno = ano;
    this.mCommand = command;
    this.mNameIndex = indexname;
    this.mLayno = layno;
    this.mPro = pro; 
}

//インデックスボタン処理
function cIndex(no) {
    var ifr = document.getElementById("sframe").contentWindow;

    if (bNo != no) {
        pauseVideo();

        dLayout = gScriptTable[no - 1].mLayno;

        zoomcheck(no - 1);

        msg = "jumpToAnimation:" + gScriptTable[no - 1].mSno + ":" + gScriptTable[no - 1].mAno + ":0"
        if (window.addEventListener) {
            ifr.postMessage(msg, "*");
        } else {
            ifr[msg]()
        }
        mMark = gScriptTable[no - 1].mNo;
        mSnam = gScriptTable[no - 1].mSno;

        var vTime = gScriptTable[no - 1].mTime;
        if (drawflag == 1) {
            drawinit(vTime);
            clearPic();
        }

        arView(gScriptTable[no - 1].mNo);

        v.currentTime(vTime);
        playVideo();

        bNo = no;
    }
}

//シークバーによるスライド変更処理
function sIndex(no) {
    var ifr = document.getElementById("sframe").contentWindow;

    if (bNo != no) {
        pauseVideo();

        var y = no;

        dLayout = gScriptTable[y - 1].mLayno;

        for (r = 0; y > r; y--) {
            if (gScriptTable[y - 1].mCommand == "zoomin") {
                fZoom = 1;
                break;
            } else if (gScriptTable[y - 1].mCommand == "zoomout") {
                fZoom = 0;
                break;
            }
            fZoom = 0;
        }

        if (fZoom == 1) {
            if (dLayout == "999") {
                ChangeLayout(gScriptTable[no - 2].mLayno);
                ChangeLayout("999");
            } else {
                ChangeLayout(dLayout);
                ChangeLayout("999");
            }
        } else {
            ChangeLayout(dLayout);
        }

        msg = "jumpToAnimation:" + gScriptTable[no - 1].mSno + ":" + gScriptTable[no - 1].mAno + ":0"
        if (window.addEventListener) {
            ifr.postMessage(msg, "*");
        } else {
            ifr[msg]()
        }
        mMark = gScriptTable[no - 1].mNo;
        mSnam = gScriptTable[no - 1].mSno;

        //var vTime = gScriptTable[no - 1].mTime;
        //var vTime = v.currentTime(); 
        //if (drawflag == 1) {
        //    drawinit(vTime);
        //    drawclear();
        //    clearPic();
        //}

        arView(gScriptTable[no - 1].mNo);

        bNo = no;
    }
}

//シークバー操作処理
function sPosex() {
    var ifr = document.getElementById("sframe").contentWindow;

    //現在時間の取得
    var pt = v.currentTime();

    //timeline.jsからインデックスを取得
    var f = gScriptTable.length;

    if (drawflag == 1) {
        drawinit(pt);
        drawclear();
        clearPic();
    }
    
    //a時間以上b時間未満を検出]
    for (var j = 0; j < f; j++) {
        if (pt >= gScriptTable[j].mTime) {
            if ((j+1) == f) {
                sIndex(gScriptTable[j].mNo);
                return;
            }

            if (pt <= gScriptTable[j + 1].mTime) {
                sIndex(gScriptTable[j].mNo);
                return;
            }
        }
    }
}

//時間経過による変更処理
function sPos() {
    var ifr = document.getElementById("sframe").contentWindow;

    //現在時間の取得
    var pt = v.currentTime();

    if (drawflag == 1) {

        //手書き描画時間の場合は手書き処理を開始
        if (pt >= drawtime[drawIndex]) {
            drawfunc();
        }
    }

    //timeline.jsからインデックスを取得
    var f = gScriptTable.length;

    //a時間以上b時間未満を検出]
    for (var j = 0; j < f; j++) {
        if (pt >= gScriptTable[j].mTime) {
            if (mMark < gScriptTable[j].mNo) {

                if (mSnam != gScriptTable[j].mSno) {

                    if (drawflag == 1)
                        clearPic();

                    arView(gScriptTable[j].mNo);
                }

                switch (gScriptTable[j].mCommand) {
                    case "flipslide":
	                    if (fScorm) {
   							if(gScriptTable[j].mPro) {
     							doScore(((gScriptTable[j].mSno) - 1) / (gScriptTable[f - 1].mSno) * 100);
   							}
						}
                        if (!fZoom) {
                            dLayout = gScriptTable[j].mLayno;
                            ChangeLayout(gScriptTable[j].mLayno);
                        } else {
                            dLayout = gScriptTable[j].mLayno;
                            ChangeLayout(gScriptTable[j].mLayno);
                            ChangeLayout("999");
                        }
                        msg = "stepAnimation"
                        if (window.addEventListener) {
                            ifr.postMessage(msg, "*");
                        } else {
                            ifr[msg]()
                        }
                        break;

                    case "demo":
                        if(pt <= gScriptTable[j+1].mTime){
                            pauseVideo();
                            demostart(gScriptTable[j].mNameIndex);
                        }
                        break;

                    case "zoomin":

                        fZoom = 1;
                        dLayout = gScriptTable[j - 1].mLayno;
                        ChangeLayout(gScriptTable[j].mLayno);

                        loadPic();
                        break;

                    case "zoomout":

                        loadPic();

                        fZoom = 0;
                        dLayout = gScriptTable[j].mLayno;
                        ChangeLayout(gScriptTable[j].mLayno);
                        break;

                    case "draw":
                        break;
                }
                mMark = gScriptTable[j].mNo;
                mSnam = gScriptTable[j].mSno;
            }
        }
    }
}

//zoominとzoomoutチェック
function zoomcheck(ino) {

    var y = ino;

    for (r = 0; y > r; y--) {
        if (gScriptTable[y].mCommand == "zoomin") {
            fZoom = 1;
            ChangeLayout(gScriptTable[ino].mLayno);
            ChangeLayout("999");
            return;
        } else if (gScriptTable[y].mCommand == "zoomout") {
            fZoom = 0;
            ChangeLayout(gScriptTable[ino].mLayno);
            return;
        }
    }

    fZoom = 0;
    ChangeLayout(gScriptTable[ino].mLayno);
}

//手書きインデックスの初期化
function drawinit(dTi) {
    for (var u = draw.length; u >= 0; u--) {
        if (dTi > drawtime[u]) {
            break;
        }
        drawIndex = u;
    }
}