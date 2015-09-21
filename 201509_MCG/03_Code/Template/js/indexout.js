
    //インデックス時間表示のため時間取得
    var gtime = [];
    if (iSlideTime)
    {
        for (var i = 0; i < gScriptTable.length; i++)
        {
            if (gScriptTable[i].mPro)
            {
                gtime.push(gScriptTable[i].mTime);
            }
        }
        gtime.push(vduration);
    }


    function ChangeTab(tabname)
    {
        // 全部消す
        document.getElementById('tab1').style.display = 'none';
        document.getElementById('tab2').style.display = 'none';
        // 指定箇所のみ表示
        document.getElementById(tabname).style.display = 'block';
    }

    function indexstyle()
    {
        document.writeln('<style type=\"text/css\"><!--');
        document.writeln('/* 表示領域全体 */');
        document.writeln('div.tabbox {');
        document.writeln('   margin: 0px; padding: 0px; width:' + (vsizex - 5) + 'px;');
        document.writeln('}');

        document.writeln('/* タブ部分 */');
        document.writeln('p.tabs { margin: 0px; padding: 0px; }');
        document.writeln('p.tabs a {');
        document.writeln('   display: block; width: 5em; float: left;');
        document.writeln('   margin: 0px 1px 0px 0px; padding: 3px;');
        document.writeln('   text-align: center;');
        document.writeln('}');
        document.writeln('p.tabs a.tab1 { background-color: ' + bTabcolor1 + '; color: white; }');
        document.writeln('p.tabs a.tab2 { background-color: ' + bTabcolor2 + '; color:white;}');
        document.writeln('p.tabs a:hover { color: yellow; }');
        document.writeln('/* タブ中身のボックス */');
        document.writeln('div.tab { height: ' + (wsizey - vsizey - 45) + 'px; overflow: auto; clear: left; }');
        document.writeln('div#tab1 {');
        document.writeln('   border: 2px solid ' + bTabcolor1 + '; background-color: #ffffff;');
        document.writeln('}');
        document.writeln('div#tab2 {');
        document.writeln('  border: 2px solid ' + bTabcolor2 + '; background-color: #ffffff;');
        document.writeln('}');
        document.writeln('div.tab p { margin: 0.5em; }');
        document.writeln('td');
        document.writeln('{');
        document.writeln('    width: ' + (vsizex - 50) + 'px;');
        document.writeln('    border-top: none;');
        document.writeln('    border-left: none;');
        document.writeln('    border-right: none;');
        document.writeln('    border-bottom: medium solid #cccccc;');
        document.writeln('}');
        document.writeln('td:hover');
        document.writeln('{');
        document.writeln('    background: #eeeeff;');
        document.writeln('}');
        document.writeln('img.thumb');
        document.writeln('{');
        document.writeln('    max-width: 100px;');
        document.writeln('    max-height: 100px;');
        document.writeln('}');
        document.writeln('img.arrow');
        document.writeln('{');
        document.writeln('    max-width: 20px;');
        document.writeln('    max-height: 20px;');
        document.writeln('}');
        document.writeln('--></style>');
    }
    
    function indexwrite()
    {
        document.writeln('<div class=\"tabbox\">');
        document.writeln('   <p class=\"tabs\">');
        //タブ名の変更
        document.writeln('      <a href=\"#tab1\" class=\"tab1\" onclick=\"ChangeTab(\'tab1\'); return false;\">目次</a>');
        if (iMessage) {
            document.writeln('      <a href=\"#tab2\" class=\"tab2\" onclick=\"ChangeTab(\'tab2\'); return false;\">情報</a>');
        } else {
            document.writeln('      <a href=\"#tab2\" class=\"tab2\" style=\"display: none;\">情報</a>');
        }
        document.writeln('   </p>');
        document.writeln('   <div id=\"tab1\" class=\"tab\">');

        //インデックスの内容をフィードバック
        //インデックスにサムネイル画像を表示処理(できれば)
        if (gScriptTable.length) {
            var qTime = 0;
            var r = 1;
            var inam;
            document.writeln('    <table>');
            for (var i = 0; i < gScriptTable.length; i++) {
                if (gScriptTable[i].mPro) {
                    if (iNumber) {
                        inam = r + '. ' + gScriptTable[i].mNameIndex;
                    } else {
                        inam = gScriptTable[i].mNameIndex;
                    }
                    document.writeln('        <tr onclick="cIndex(id);return false;" id="' + gScriptTable[i].mNo + '">');
                    var tdstr = "            <td><img class=\"arrow\" id=\"ar" + gScriptTable[i].mNo + "\" src=\"iArrow.png\" />";
                    if (iThumb) {
                        tdstr = tdstr + "<img class=\"thumb\" src=\"THUMB1/Slide" + gScriptTable[i].mSno + ".png\" border=\"1\"/>";
                    }
                    if (iSlideTime) {
                        var tstr = timestring((gtime[r] - qTime));
                        document.writeln(tdstr + inam + '    (' + tstr + ')' + '</td>');
                        qTime = gtime[r];
                    } else {
                        document.writeln(tdstr + inam + '</td>');
                    }
                    document.writeln('        </tr>');
                    r++;
                }
            }
            document.writeln('    </table>');
        }

        document.writeln('   </div>');
        document.writeln('   <div id=\"tab2\" class=\"tab\">');

        messagetext();

        document.writeln('   </div>');
        document.writeln('</div>');
    }

    function timestring(tv)
    {
        tt = tv;
        th = (Math.floor(tt / 3600));
        tm = (Math.floor((tt - th * 3600) / 60));
        ts = (tt - th * 3600 - tm * 60);
        var timestr = ('0' + th).slice(-2) + ':' + ('0' + tm).slice(-2) + ':' + ('0' + ts).slice(-2);
        return timestr;
    }

    function arView(ar)
    {
        for (var t = 0; t < gScriptTable.length; t++) {
            if (gScriptTable[t].mPro) {
                var s = "ar" + (t+1);
                if ((t+1) == ar) {
                    document.getElementById(s).style.display = 'inline';
                } else {
                    document.getElementById(s).style.display = 'none';
                }
            }
        }
    }