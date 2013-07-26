<?php
 
function get_data($url) {
    $ch = curl_init();
    $timeout = 5;
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, $timeout);
    $data = curl_exec($ch);
    curl_close($ch);
    return $data;
}

define('API_KEY', 'API_KEY_GOES_HERE'); 
$payload = json_decode(file_get_contents('php://input'), true);
if(!$payload) die();
$payload[1] /= 10000;
$payload[2] /= 10000;

$contents = get_data("https://api.forecast.io/forecast/API_KEY_GOES_HERE/$payload[1],$payload[2]?units=$payload[3]&exclude=hourly,minutely,alerts");

$forecast = json_decode($contents);
 
if(!$forecast) {
    die();
}      
$response = array();
$icons = array(
    'clear-day' => 0,
    'clear-night' => 1,
    'rain' => 2,
    'snow' => 3,
    'sleet' => 4,
    'wind' => 5,
    'fog' => 6,
    'cloudy' => 7,
    'partly-cloudy-day' => 8,
    'partly-cloudy-night' => 9
);
$sunset = $forecast->daily->data[0]->sunsetTime;
$sunset_h = date('H', $sunset);
$sunset_m = date('i', $sunset);
$icon_id = $icons[$forecast->currently->icon];
$response[1] = array('b', $icon_id);
$response[2] = array('s', round($forecast->currently->temperature));
$response[3] = array('s', round($forecast->daily->data[0]->temperatureMax));
$response[4] = array('s', round($forecast->daily->data[0]->temperatureMin));
$response[5] = array('s', intval($sunset_h));
$response[6] = array('s', intval($sunset_m));
header("Cache-Control: max-age=540");
print json_encode($response);
