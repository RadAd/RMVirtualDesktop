$target = $args[0]

Write-Output "Writing footer..."
$size = [long](Get-Item $target).length
$size_bytes = [System.BitConverter]::GetBytes($size)
Add-Content -Path $target -Value $size_bytes -Encoding Byte

$flags = [byte]0
Add-Content -Path $target -Value $flags -Encoding Byte

$rmskin = [string]"RMSKIN`0"
Add-Content -Path $target -Value $rmskin -NoNewLine -Encoding ASCII

Write-Output "Changing .zip to .rmskin"
Rename-Item -Path $target -NewName ([io.path]::ChangeExtension($target, '.rmskin'))
$target = $target.Replace(".zip", ".rmskin")
