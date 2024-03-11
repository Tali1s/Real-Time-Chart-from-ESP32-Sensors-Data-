<?php
$servername = "localhost";
// ** Replace with your Database name **
$dbname = "id21657821_esp_data";
// ** Replace with Database user **
$username = "id21657821_esp_data";
// ** Replace with Database user password **
$password = "Asdasd@123456";

$conn = new mysqli($servername, $username, $password, $dbname);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT id, value1, value2,  reading_time FROM Sensor ORDER BY reading_time DESC LIMIT 40";
$result = $conn->query($sql);

$sensor_data = array();

while ($data = $result->fetch_assoc()){
    $sensor_data[] = $data;
}

$readings_time = array_column($sensor_data, 'reading_time');

$value1 = array_reverse(array_column($sensor_data, 'value1'));
$value2 = array_reverse(array_column($sensor_data, 'value2'));
$reading_time = array_reverse($readings_time);

$response = array(
    'value1' => $value1,
    'value2' => $value2,
    'reading_time' => $reading_time
);

header('Content-Type: application/json');
echo json_encode($response, JSON_NUMERIC_CHECK);

$result->free();
$conn->close();
?>
