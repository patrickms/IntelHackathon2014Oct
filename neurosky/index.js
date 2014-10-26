var neurosky = require('node-neurosky')
var Firebase = require('firebase')
var myFirebaseRef = new Firebase("https://blazing-fire-1158.firebaseio.com/robosapien");

var client = neurosky.createClient({
	appName:'NodeNeuroSky',
	appKey:'0fc4141b4b45c675cc8d3a765b8d71c5bde9390'
})

client.on('data',function(data){
	console.log(data) //"76"
	if(data.blinkStrength)
	{
	}
	else
	{
		if(data.poorSignalLevel==0)
		{
			if(data.eSense)
			{
				if(data.eSense.meditation>75)
				{
					console.log("Walk Robosapien!");
					myFirebaseRef.set("76");
				} else
				{
					console.log("Meditate to make Robosapien walk!");
					myFirebaseRef.set("-1");
				}
			}
		}
	}
});

client.connect()
