var lay999 = function () {
};

lay999.prototype = {
    setVarea: function () {
        var vdv = document.getElementById("videoarea");
        var sdv = document.getElementById("slidearea");
        var sfr = document.getElementById("sframe");
        var pcon = document.getElementsByClassName("vjs-control-bar");
        pcon[0].style.display = "none";
        vdv.style.display = "none";

        destwidth = v.width();
        destheight = v.height();
        desttop = vdv.style.top;
        destleft = vdv.style.left;

        v.width(sfr.width);

        switch (dLayout) {
            case "007":
            case "008":
            case "011":
            case "012":
            case "015":
            case "016":
            case "021":
            case "022":
                v.height(sfr.height);
                break;
            default:
                v.height((sfr.height - 30));
                break;
        }

        vdv.style.top = sdv.style.top;
        vdv.style.left = sdv.style.left;

        pcon[0].style.top = "";
        pcon[0].style.width = "";

        if (vAdviser) {
            pcon[0].style.top = v.height() + 8 + "px";
        }

        switch (dLayout) {
            case "009":
            case "013":
                vdv.style.top = wsizey - topy - 100 + "px";
                vdv.style.left = vsizex + topx + topx + "px";
                v.height(1);
                v.width(1);

                if (vAdviser) {
                    pcon[0].style.top = "78px";
                } else {
                    pcon[0].style.top = "70px";
                }

                pcon[0].style.left = "0px";
                pcon[0].style.width = wsizex - topx - topx - topx - vsizex + "px";
                break;
            case "010":
            case "014":
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
                pcon[0].style.width = wsizex - topx - topx - topx - vsizex + "px";
                break;
            case "017":
                vdv.style.top = wsizey - topy - (wsizey * 0.1 - topy) - 100 + "px";
                vdv.style.left = topx + topx + vsizex + "px";
                v.height(1);
                v.width(1);

                if (vAdviser) {
                    pcon[0].style.top = "73px";
                } else {
                    pcon[0].style.top = "70px";
                }

                pcon[0].style.left = "0px";
                pcon[0].style.width = wsizex - (topx + topx + topx + vsizex) + "px";
                break;
            case "018":
                vdv.style.top = wsizey - topy - (wsizey * 0.1 - topy) - 100 + "px";
                vdv.style.left = topx + "px";
                v.height(1);
                v.width(1);

                if (vAdviser) {
                    pcon[0].style.top = "73px";
                } else {
                    pcon[0].style.top = "70px";
                }

                pcon[0].style.left = "0px";
                pcon[0].style.width = wsizex - (topx + topx + topx + vsizex) + "px";
                break;
            case "019":
            case "020":
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
                break;
            case "023":
                v.height(sfr.height);
                pcon[0].style.width = wsizex - topx - topx + "px";
                pcon[0].style.left = (-1 * (wsizex / 2 - topy)) + "px";
                break;
        }

        vdv.style.display = "block";
        pcon[0].style.display = "block";

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

    setParea: function () {
    },

    setLarea: function () {
    },

    setInarea: function () {
    },

    setSarea: function () {
        var sdv = document.getElementById("slidearea");
        var sfr = document.getElementById("sframe");
        sdv.style.display = "none";
        sfr.style.display = "none";

        sdv.style.top = desttop;
        sdv.style.left = destleft;
        sfr.height = destheight;
        sfr.width = destwidth;

        sdv.style.display = "block";
        sfr.style.display = "block";
    },

    setTiarea: function () {
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

        cdv.style.top = parseInt(desttop) + (sfr.clientHeight / 2) - (ssizey * zoomp() / 2) + "px";
        cdv.style.left = parseInt(destleft) + (sfr.clientWidth / 2) - (ssizex * zoomp() / 2) + "px";
        cdv.style.height = ssizey * zoomp() + "px";
        cdv.style.width = ssizex * zoomp() + "px";

        canvas.style.zoom = zoomp();
        b_canvas.style.zoom = zoomp();
        if (brow() == "firefox") {
            //firefoxのみzoomが無効のため別処理
            var trafx = (-1 * ((ssizex - (ssizex * zoomp())) / 2 / zoomp()));
            var trafy = (-1 * ((ssizey - (ssizey * zoomp())) / 2 / zoomp()));
            canvas.style.transform = "scale(" + zoomp() + ", " + zoomp() + ")"
                                    + "translate(" + trafx + "px, " + trafy + "px)";
        }
        if (brow() == "firefox") {
            //firefoxのみzoomが無効のため別処理
            var trafx = (-1 * ((ssizex - (ssizex * zoomp())) / 2 / zoomp()));
            var trafy = (-1 * ((ssizey - (ssizey * zoomp())) / 2 / zoomp()));
            b_canvas.style.transform = "scale(" + zoomp() + ", " + zoomp() + ")"
                                    + "translate(" + trafx + "px, " + trafy + "px)";
        }
    }
};