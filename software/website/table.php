 <?php
  include_once("dbconnect.php");
	 
  $hasil = mysqli_query($dbc, "SELECT * FROM logsensor ORDER BY id_data DESC LIMIT 10");
  while ($baris = mysqli_fetch_row($hasil)) 
  {    
    $id_data    = $baris[0];
    $waktu      = $baris[1];  
    $turbidity  = $baris[2];

    //print("$id_data, $waktu, $turbidity<br>");
    print("$waktu, $turbidity<br>");
  }

  exit();
?>