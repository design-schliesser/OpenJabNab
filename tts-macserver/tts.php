<?php
/********************************************************/
/* Diese Datei ist Teil des Rabbit-Island Projektes.	*/
/* Copyright by André Schließer und Hochschule Ulm		*/
/* 														*/
/* Dieses Script funktioniert nur auf Mac OS X Servern	*/
/* korrekt, da es die Terminalfunktion "say" verwendet.	*/
/********************************************************/
//error_reporting(E_ALL);

// Config
$checkFolders = false;
$ttsFolder = "tts"; //Must be writeable! Just for temporary generated files!
$logFolder = "log"; //Folder for the log files
$filename = "";


// List of installed Voices
$voicelist = array("Anna", "Markus", "Petra", "Yannick", 
"Alex", "Bruce", "Fred", "Junior", "Ralph", "Tom", 
"Agnes", "Allison", "Ava", "Kathy", "Princess", "Samantha", "Susan", "Vicki", "Victoria", 
"Albert", "Bad", "Bad News", "Bahh", "Bells", "Boing", "Bubbles", "Cellos", "Deranged", "Good", "Good News", "Hysterical", "Pipe", "Pipe Organ", "Trinoids", "Whisper", "Zarvox");


//Check folders
if($checkFolders){
	folderCheck($logFolder);
	folderCheck($ttsFolder);
}

// Get the voice
if(isset($_GET['v'])){
	$voice = $_GET['v'];
	if(preg_grep('/'.$voice.'/i',$voicelist)){ // replaced "in_array()" because it isn't caseinsensitive
		if(strtolower($voice) == "bad news"){ // shorten strings with two words
			$voice = "Bad";
		}elseif(strtolower($voice) == "good news"){
			$voice = "Good";
		}elseif(strtolower($voice) == "pipe organ"){
			$voice = "Pipe";
		}
	} else { $voice = "anna"; }
} else { $voice="anna"; }

// Get the text
if (isset($_GET['t'])){
	$t=$_GET['t'];
	if(strlen($t) >= 1){
		$filename = md5($text.$voice); //generate unique filename
		$text = addslashes($t); // save escaped(security) text
		generateMp3($voice,$text); //generate mp3 file
	}else { logExit ("Text is to short");}
} else { logExit("Can't get Text!");}




function generateMp3($voice, $text){
	global $filename, $ttsFolder; // access variable $ttsFolder
	$say = 'say -v '.$voice.' -o '.$ttsFolder.'/'.$filename.'.aiff "'.$text.'"';
	exec($say); // execute say command and save it
	exec('lame '.$ttsFolder.'/'.$filename.'.aiff '.$ttsFolder.'/'.$filename.'.mp3'); // convert *.aiff to *.mp3
	unlink($ttsFolder.'/'.$filename.'.aiff'); // delete *.aiff
	header("Content-Type: audio/mpeg"); // set content-type for mp3
	readfile($ttsFolder.'/'.$filename.'.mp3'); // return the mp3 file
	
	unlink($ttsFolder.'/'.$filename.'.mp3');  // delete *.mp3
	logTTS("Success! $say"); // create log entry
}

function folderCheck($foldername){
	if(!file_exists($foldername)){ // Check if folder "tts" exists
		if(!mkdir($foldername, 0777, true)){ // try to create folder tts
			logExit("Can't create folder $foldername!"); // else return an error
		}
	}
}

// Write every Action to the logfile
function logTTS($error = "No Error"){
	global $logFolder; // access variable $logFolder
	$ip=getenv("REMOTE_ADDR"); // Get IP address
	$site = $_SERVER['REQUEST_URI']; // Get requested URL-params
	$logfileame=$logFolder."/ttslog_".date('Ymd').".txt"; // Create every day a new logfile
	$log="[".date('d.m.Y H:i:s')."] [$ip] - $error - \"GET $site\""; // Create log entry
	$datei=fopen($logfileame,"a"); // Open/Create file and append
	fputs($datei,"$log\n"); // Write log file
	fclose($datei); //Close log file
}

// Abort and Log on Error
function logExit($error = "No Error"){
	logTTS($error);
	exit($error);
}
?>