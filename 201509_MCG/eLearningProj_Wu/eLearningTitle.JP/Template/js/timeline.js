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

    if (f_comp == 0) {
        return;
    }

    if (bNo != no) {

        var f = gScriptTable.length;

        zoomcheck(gScriptTable[no - 1].mNo-1);

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
            if (j +1 == f || pt < gScriptTable[j + 1].mTime) {
                if (mMark != gScriptTable[j].mNo) {

                    var f_anime = 0;

                    if (mSnam != gScriptTable[j].mSno) {

                        f_anime = 1;

                        if (drawflag == 1)
                            clearPic();

                        if (gScriptTable[j].mPro) {
                            arView(gScriptTable[j].mNo);
                        } else {
                            var r = j;
                            while (!gScriptTable[r].mPro) {
                                r--;
                                if (gScriptTable[r].mPro) {
                                    arView(gScriptTable[r].mNo);
                                }
                            }
                        }
                    } else if (mAnam != gScriptTable[j].mAno) {

                        f_anime = 1;

                    }

                    switch (gScriptTable[j].mCommand) {
                        case "flipslide":
                            if (fScorm) {
                                if (gScriptTable[j].mPro) {
                                    doScore(((gScriptTable[j].mSno) - 1) / (gScriptTable[f - 1].mSno) * 100);
                                }
                            }

                            if (drawflag == 1) {
                                if (gScriptTable[j].mPro) {
                                    drawinit(vTime);
                                    clearPic();
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

                            msg = "jumpToAnimation:" + gScriptTable[j].mSno + ":" + gScriptTable[j].mAno + ":0";

                            if (window.addEventListener) {
                                ifr.postMessage(msg, "*");
                            } else {
                                ifr[msg]()
                            }
                            break;

                        case "demo":
                            if (pt <= gScriptTable[j + 1].mTime) {
                                pauseVideo();
                                demostart(gScriptTable[j].mNameIndex);
                            }
                            break;

                        case "zoomin":

                            if (f_anime) {
                                msg = "jumpToAnimation:" + gScriptTable[j].mSno + ":" + gScriptTable[j].mAno + ":0";

                                if (window.addEventListener) {
                                    ifr.postMessage(msg, "*");
                                } else {
                                    ifr[msg]()
                                }
                            }

                            fZoom = 1;
                            dLayout = gScriptTable[j - 1].mLayno;
                            ChangeLayout(gScriptTable[j].mLayno);

                            loadPic();
                            break;

                        case "zoomout":

                            if (f_anime) {
                                msg = "jumpToAnimation:" + gScriptTable[j].mSno + ":" + gScriptTable[j].mAno + ":0";

                                if (window.addEventListener) {
                                    ifr.postMessage(msg, "*");
                                } else {
                                    ifr[msg]()
                                }
                            }

                            loadPic();

                            fZoom = 0;
                            dLayout = gScriptTable[j].mLayno;
                            ChangeLayout(gScriptTable[j].mLayno);
                            break;

                        case "draw":
                            break;
                    }
                    mMark = gScriptTable[j].mNo;
                    mAnam = gScriptTable[j].mAno;
                    mSnam = gScriptTable[j].mSno;
                    break;
                }
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