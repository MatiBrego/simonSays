char webpage[] PROGMEM = R"=====(
<html>
<head>
  <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-KK94CHFLLe+nY2dmCWGMq91rCGa5gtU4mk92HdvYe+M/SXH301p5ILy+dN9+nJOZ" crossorigin="anonymous">
  <script>
    var Socket;
    function init() {
      Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
      Socket.onmessage = function(event){
        document.getElementById("rxConsole").value += event.data;
      }
    }
    function sendBtnPressed(color){
      console.log(color);
      Socket.send(color);
    }  
  </script>
</head>
<body onload="javascript:init()">
  <dic class="container align-items-center m2">
    <div class="row">
        <button type="button" class="col-sm-2 btn btn-danger" id="red" onClick=sendBtnPressed("red");>Red</button>
        <button type="button" class="col-sm-2  btn btn-primary" id="blue" onClick=sendBtnPressed("blue");>Blue</button>
    </div>
  </div>

  <br/>
  
  <dic class="container align-items-center m2">
    <div class="row">
        <button type="button" class="col-sm-2 btn btn-warning" id="yellow" onClick=sendBtnPressed("yellow");>Yellow</button>
        <button type="button" class="col-sm-2 btn btn-success" id="green" onClick=sendBtnPressed("green");>Green</button>
    </div>
  </div>

  <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha3/dist/js/bootstrap.bundle.min.js" integrity="sha384-ENjdO4Dr2bkBIFxQpeoTz1HIcje39Wm4jDKdf19U8gI4ddQ3GYNS7NTKfAdVQSZe" crossorigin="anonymous"></script>
</body>
</html>
)=====";

