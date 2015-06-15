Pebble.addEventListener('showConfiguration', function(e) {
  // Show config page
  //Pebble.openURL('http://127.0.0.1/rellotgecatala.html');
  Pebble.openURL('https://dl.dropboxusercontent.com/u/13130748/pebble/rellotgecatala.html');
});

Pebble.addEventListener("webviewclosed",
  function(e) {
    //Get JSON dictionary
    console.log(e.response);
    var configuration = JSON.parse(decodeURIComponent(e.response));
    console.log("Configuration window returned: " + JSON.stringify(configuration));
    if ( e.response.length === 0 ) {
      return;
    } else if ( configuration.reset ) {
      configuration.invert = 'off';
      configuration.first_ucase = 'ucase';
      configuration.veralign = 'center';
      configuration.horalign = 'center';
      configuration.show_senyera = 'show';
      configuration.show_date = 'show';
    }
 
    //Send to Pebble, persist there
    Pebble.sendAppMessage(
      {"KEY_INVERT": configuration.invert,
       "KEY_HORALIGN": configuration.horalign,
       "KEY_VERALIGN": configuration.veralign,
       "KEY_HIDEDATE": configuration.show_date,
       "KEY_LCASE": configuration.first_ucase,
       "KEY_HIDESENYERA": configuration.show_senyera},
      function(e) {
        console.log("Sending settings data...");
      },
      function(e) {
        console.log("Settings feedback failed!");
      }
    );
  }
);