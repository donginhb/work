var lay023 = function () {
};

lay023.prototype = {
    setVarea: function () {
        var vdv = document.getElementById("videoarea");
        var vcon = document.getElementById("video");
        var pcon = document.getElementsByClassName("vjs-control-bar");

        pcon[0].style.display = "block";
        vdv.style.display = "block";
        vcon.style.display = "block";

        pcon[0].style.top = "";
        pcon[0].style.left = "0px";
        pcon[0].style.width = wsizex - 20 + "px";

        v.width(Math.ceil(wsizex / 2 - 10));
        v.height(Math.ceil(v.width() / vsizex * vsizey));
        vdv.style.top = Math.ceil(topy + wsizey / 2 - v.height() / 2) + "px";
        vdv.style.left = topx + "px";

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
        var lpic = document.getElementById("logopic");
        var vdv = document.getElementById("videoarea");

        ldv.style.display = "block";
        lpic.style.display = "block";

        ldv.style.top = vdv.offsetTop - lpic.height - 5 + "px";
        ldv.style.left = topx + "px";
        lpic.width = wsizex - 20;
    },

    setInarea: function () {
        var indv = document.getElementById("indexarea");

        indv.style.display = "none";
    },

    setSarea: function () {
        var sdv = document.getElementById("slidearea");
        var sfr = document.getElementById("sframe");

        sdv.style.display = "block";
        sfr.style.display = "block";

        sdv.style.top = Math.ceil(topy + wsizey / 2 - Math.ceil(((wsizex / 2 - 5) / vsizex * vsizey)) / 2) + "px";
        sdv.style.left = Math.ceil(wsizex / 2) + "px";
        sfr.height = Math.ceil(Math.ceil((wsizex / 2 - 5)) / vsizex * vsizey);
        sfr.width = Math.ceil(wsizex / 2 - 10);
    },

    setTiarea: function () {
        var tdv = document.getElementById("titlearea");
        tdv.style.display = "none";
    },

    setIVarea: function () {
        //var isdv = document.getElementById("indexseek");
        //isdv.style.display = "none";
    },

    setMarea: function () {
        //var mdv = document.getElementById("mesarea");
        //mdv.style.display = "none";
    },

    setCanarea: function () {
        var canvas = document.getElementById("can1");
        var b_canvas = document.getElementById("can2");
        var cdv = document.getElementById("drawarea");
        var sfr = document.getElementById("sframe");

        cdv.style.top = (Math.ceil(topy + wsizey / 2 - Math.ceil(((wsizex / 2 - 5) / vsizex * vsizey)) / 2)) + (sfr.clientHeight / 2) - (ssizey * zoomp() / 2) + "px";
        cdv.style.left = Math.ceil(wsizex / 2) + (sfr.clientWidth / 2) - (ssizex * zoomp() / 2) + "px";
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