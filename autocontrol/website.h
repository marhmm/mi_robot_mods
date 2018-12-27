const String header = "<!DOCTYPE HTML>\r\n<html><head><title>MiRobotShutdown</title>";
const String footer = "</html>";

extern uint8_t auto_shutdown_enabled;
extern uint8_t auto_start_enabled;
extern String robot_access_error;

String get_startpage(){
  return header + \
  "</head>" + \
  "<body>" + \
    "<h1>Mi Robot Autocontrol v2.0</h1>"+ \
    "<br>"+ \
    "<span id=\"access\">" + robot_access_error + "</span>" + \
    "<br>"+ \
    "<table>" + \
      "<tr>" + \ 
        "<td>Start cleaning on power up:</td>" + \
        "<td>" + (auto_start_enabled ? "ON" : "OFF") + "</td>" + \
        "<td><a href=\"toggle_auto_start\"><button>Toggle</button></a><br></td>"+ \
        "<td>Starting in: <span id=\"auto_start_counter\"></span></td>"+ \
      "</tr><tr>" + \
        "<td>Shutdown on power loss:</td>" + \
        "<td>" + (auto_shutdown_enabled ? "ON" : "OFF") + "</td>" + \
        "<td><a href=\"toggle_auto_shutdown\"><button>Toggle</button></a><br></td>"+ \
        "<td>Shutdown in: <span id=\"auto_shutdown_counter\"></span></td>"+ \
      "</tr>" + \
      "</tr><tr>" + \
        "<td>Robot state:</td>" + \
        "<td><span id=\"data\"></span></td>" + \
        "<td></td>"+ \
        "<td></td>"+ \
      "</tr>" + \
      "</tr><tr>" + \
        "<td>Battery SOC:</td>" + \
        "<td><span id=\"SOC\"></span></td>" + \
        "<td></td>"+ \
        "<td></td>"+ \
      "</tr>" + \
    "</table>" + \
    "<br>"+ \
    "<a href=\"shutdown\"><button>Shutdown now</button></a>"+ \
    "<a href=\"update\"><button>Update firmware</button></a>"+ \
    "" + \
    
    "<script>" + \
      "var x = setInterval(function() {loadData(\"get_data\",updateData)}, 1000);" + \
      "function loadData(url, callback){" + \
        "var xhttp = new XMLHttpRequest();" + \
        "xhttp.onreadystatechange = function(){" + \
          "if(this.readyState == 4 && this.status == 200){" + \
            "callback.apply(xhttp);" + \
          "}" + \
        "};" + \
        "xhttp.open(\"GET\", url, true);" + \
        "xhttp.send();" + \
      "}" + \
      "function updateData(){" + \        
        "document.getElementById(\"data\").innerHTML = this.responseText.split(\"\\n\")[0];" + \
        "document.getElementById(\"access\").innerHTML = this.responseText.split(\"\\n\")[1];" + \
        "document.getElementById(\"SOC\").innerHTML = this.responseText.split(\"\\n\")[2];" + \
        "document.getElementById(\"auto_start_counter\").innerHTML = this.responseText.split(\"\\n\")[3];" + \
        "document.getElementById(\"auto_shutdown_counter\").innerHTML = this.responseText.split(\"\\n\")[4];" + \
      "}" + \
    "</script>" + \
  "</body>" + \
  footer;
}
