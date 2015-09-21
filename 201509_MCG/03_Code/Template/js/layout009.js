var lay009 = function () {
};

lay009.prototype = {
    setVarea: function () {
        var vdv = document.getElementById("videoarea");
        var vcon = document.getElementById("video");
        var pcon = document.getElementsByClassName("vjs-control-bar");

        pcon[0].style.display = "block";
        vdv.style.display = "block";
        vcon.style.display = "block";

        vdv.style.top = topy + "px";
        vdv.style.left = topx + topx + vsizex + "px";
        v.height(wsizey - 50);
        v.width(wsizex - (topx + topx + vsizex) - 5);

        pcon[0].style.top = "";
        pcon[0].style.left = "0px";
        pcon[0].style.width = "";

        if (vAdviser) {
            var adv = document.getElementById("adarea");
            var adpic = document.getElementById("adpic");

            adv.style.display = "block";

            pcon[0].style.top = v.height() + 8 + "px";

            adpic.width = v.width();
            adpic.height = v.height();
            adv.style.top = vdv.style.top;
            adv.style.left = vdv.style.left;
        }
    },

    setLarea: function () {
        var ldv = document.getElementById("logoarea");
        ldv.style.display = "none";
    },

    setInarea: function () {
        var indv = document.getElementById("indexarea");
        var intab1 = document.getElementById("tab1");
        var intab2 = document.getElementById("tab2");
        var tdvh = document.getElementById("titlearea").clientHeight;

        indv.style.display = "block";
        
        indv.style.top = topy + tdvh + "px";
        indv.style.left = topx + "px";
        intab1.style.height = wsizey - (topy + tdvh) - 35 + "px";
        intab2.style.height = wsizey - (topy + tdvh) - 35 + "px";
    },

    setSarea: function () {
        var sdv = document.getElementById("slidearea");
        var sfr = document.getElementById("sframe");

        sdv.style.display = "none";
    },

    setTiarea: function () {
        var tdv = document.getElementById("titlearea");

        tdv.style.display = "block";

        tdv.innerHTML = "<a id = \'str_title\'>" + TitleName + "</a>";

        var ta = document.getElementById("str_title");

        ta.style.fontSize = TitleSize;
        ta.style.fontFamily = TitleFont;
        ta.style.color = Titlecolor;

        var tx = tdv.clientWidth;

        if (tx > vsizex) {
            tdv.style.width = vsizex + "px";
            tdv.style.left = topx + "px";
        } else {
            tdv.style.left = ((vsizex + 10) / 2) - (tx / 2) + "px";
        }

        tdv.style.top = topy + "px";
    },

    setIVarea: function () {
    },

    setMarea: function () {
    },

    setCanarea: function () {
        var canvas = document.getElementById("can1");
        var b_canvas = document.getElementById("can2");
        var cdv = document.getElementById("drawarea");
        var sfr = document.getElementById("sframe");

        cdv.style.top = topy + (sfr.clientHeight / 2) - (ssizey * zoomp() / 2) + "px";
        cdv.style.left = (topx + vsizex) + (sfr.clientWidth / 2) - (ssizex * zoomp() / 2) + "px";
        cdv.style.height = ssizey * zoomp() + "px";
        cdv.style.width = ssizex * zoomp() + "px";
        canvas.height = ssizey;
        canvas.width = ssizex;
        canvas.style.zoom = zoomp();
        if (brow() == "firefox") {
            //firefoxのみzoomが無効のため別処理
            var trafx = (-1 * ((ssizex - (ssizex * zoomp())) / 2 / zoomp()));
            var trafy = (-1 * ((ssizey - (ssizey * zoomp())) / 2 / zoomp()));
            canvas.style.transform = "scale(" + zoomp() + ", " + zoomp() + ")"
                                    + "translate(" + trafx + "px, " + trafy + "px)";
        }

        b_canvas.height = ssizey;
        b_canvas.width = ssizex;
        b_canvas.style.zoom = zoomp();
        if (brow() == "firefox") {
            //firefoxのみzoomが無効のため別処理
            var trafx = (-1 * ((ssizex - (ssizex * zoomp())) / 2 / zoomp()));
            var trafy = (-1 * ((ssizey - (ssizey * zoomp())) / 2 / zoomp()));
            b_canvas.style.transform = "scale(" + zoomp() + ", " + zoomp() + ")"
                                    + "translate(" + trafx + "px, " + trafy + "px)";
        }
    }
};