<?php
include_once("dbconnect.php");

  $ok = true;
  if (! isset($_GET['x']))
    $ok = false;
  
  if (! $ok)
  {
     print("Salah penggunaan!");
     exit();
  }
  
  $waktu = date("Y-m-d H:i:s");
  $turbidity = $_GET['x'];
  
  $sql = "INSERT INTO logsensor (waktu, turbidity) " .
         "VALUES ('" .  $waktu. "','" . $turbidity . "');";
		 
  $hasil = mysqli_query($dbc, $sql);
  if ($hasil)
    print("Data berhasil disimpan");
  else
	print("Data gagal disimpan");  

  exit();
?>