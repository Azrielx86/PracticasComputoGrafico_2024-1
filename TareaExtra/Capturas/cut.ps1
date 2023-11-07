$imagenes = Get-ChildItem -Filter "*.png" | Where-Object { $_.Name -Match '^Screenshot' }

foreach ($imagen in $imagenes) {
    $indice = $imagenes.IndexOf($imagen)
    $outputName = "Wall_$($indice.ToString("D2")).png"
    Write-Output "Recortando: $($imagen.Name)"
    magick $imagen -crop 1220x550+28+85 -resize 512x512\! -brightness-contrast 38x47 $outputName
    Write-Output "Imagen recortada: $($outputName)"
}