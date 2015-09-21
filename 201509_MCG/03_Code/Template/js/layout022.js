var lay022 = function () {
};

lay022.prototype = {
    setVarea: function () {
        var vdv = document.getElementById("videoarea");
        var vcon = document.getElementById("video");
        var pcon = document.getElementsByClassName("vjs-control-bar");

        pcon[0].style.display = "block";
        vdv.style.display = "block";
        vcon.style.display = "block";

        vdv.style.top = wsizey - topy - 100 + "px";
        vdv.style.left = topx + "px";
        v.height(1);
        v.width(1);

        if (vAdviser) {
            pcon[0].style.top = "78px";
        } else {
            pcon[0].style.top = "70px";
        }
        pcon[0].style.left = "0px";
        pcon[0].style.width = wsizex - topx - topx + "px";

        if (vAdviser) {
            var adv = document.getElementById("adarea");
            var adpic = document.getElementById("adpic");

            adv.style.display = "block";

            adpic.width = v.width();
            adpic.height = v.height();
            adv.style.top = vdv.style.top;
            adv.style.left = vdv.style.left;
        }
    },

    setLarea: function () {
        var ldv = document.getElementById("logoarea");
        var lpic = document.getElementById("logopic");

        ldv.style.display = "block";
        lpic.style.display = "block";

        ldv.style.top = topy + "px";
        ldv.style.left = topx + "px";
        lpic.height = wsizey * 0.1 - topy;
        lpic.width = (wsizex - topx - topx);
    },

    setInarea: function () {
        var indv = document.getElementById("indexarea");

        indv.style.display = "none"; ;
    },

    setSarea: function () {
        var sdv = document.getElementById("slidearea");
        var sfr = document.getElementById("sframe");

        sdv.style.display = "block";
        sfr.style.display = "block";

        sdv.style.top = (wsizey * 0.1 + topy) + "px";
        sdv.style.left = topx + "px";
        sfr.height = wsizey * 0.9 - 50;
        sfr.width = wsizex - topx - topx;
    },

    setTiarea: function () {
        var tdv = document.getElementById("titlearea");

        tdv.style.display = "none";
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

        cdv.style.top = (wsizey * 0.1 + topy) + (sfr.clientHeight / 2) - (ssizey * zoomp() / 2) + "px";
        cdv.style.left = topx + (sfr.clientWidth / 2) - (ssizex * zoomp() / 2) + "px";
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