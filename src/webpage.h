char webpage[] PROGMEM = R"=====(
<html>
<head>
  <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-KK94CHFLLe+nY2dmCWGMq91rCGa5gtU4mk92HdvYe+M/SXH301p5ILy+dN9+nJOZ" crossorigin="anonymous">
  <script>
    var Socket;
    function init() {
      Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
      Socket.onmessage = function(event){
        console.log(event)
        document.getElementById("score").innerHTML = "Score: " + event.data ;
      }
    }
    function sendBtnPressed(color){
      console.log(color);
      Socket.send(color);
    }  
  </script>
</head>
<body className="position-relative h-100 w-100 d-flex "onload="javascript:init()">
<h1 id="score" class=" d-flex justify-content-center mt-3">Score: </h1>
  <div class="position-absolute top-50 start-50 translate-middle h-75 w-100 ">
  
  <div class="container align-items-center h-50">
    <div class="row h-100 ">
        <button type="button" class="col-sm-2 btn btn-danger w-50 fs-1 rounded-4 " id="red" onClick=sendBtnPressed("red");>Red</button>
        <button type="button" class="col-sm-2  btn btn-primary w-50 fs-1 rounded-4" id="blue" onClick=sendBtnPressed("blue");>Blue</button>
    </div>
  
  </div>

  <br/>
  
  <div class="container align-items-center  h-50">
    <div class="row h-100">
        <button type="button" class="col-sm-2 btn btn-warning w-50 fs-1 text-white rounded-4" id="yellow" onClick=sendBtnPressed("yellow");>Yellow</button>
        <button type="button" class="col-sm-2 btn btn-success w-50 fs-1 rounded-4" id="green" onClick=sendBtnPressed("green");>Green</button>
    </div>
  </div>
  </div>

  <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha3/dist/js/bootstrap.bundle.min.js" integrity="sha384-ENjdO4Dr2bkBIFxQpeoTz1HIcje39Wm4jDKdf19U8gI4ddQ3GYNS7NTKfAdVQSZe" crossorigin="anonymous"></script>
</body>
</html>
)=====";