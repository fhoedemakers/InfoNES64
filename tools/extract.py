from bs4 import BeautifulSoup

html = """
<tbody><tr>
<td style="border:0px;background-color:#626262;width:32px;height:32px;color:#fff;text-align:center">$00
</td>
<td style="border:0px;background-color:#001FB2;width:32px;height:32px;color:#fff;text-align:center">$01
</td>
<td style="border:0px;background-color:#2404C8;width:32px;height:32px;color:#fff;text-align:center">$02
</td>
<td style="border:0px;background-color:#5200B2;width:32px;height:32px;color:#fff;text-align:center">$03
</td>
<td style="border:0px;background-color:#730076;width:32px;height:32px;color:#fff;text-align:center">$04
</td>
<td style="border:0px;background-color:#800024;width:32px;height:32px;color:#fff;text-align:center">$05
</td>
<td style="border:0px;background-color:#730B00;width:32px;height:32px;color:#fff;text-align:center">$06
</td>
<td style="border:0px;background-color:#522800;width:32px;height:32px;color:#fff;text-align:center">$07
</td>
<td style="border:0px;background-color:#244400;width:32px;height:32px;color:#fff;text-align:center">$08
</td>
<td style="border:0px;background-color:#005700;width:32px;height:32px;color:#fff;text-align:center">$09
</td>
<td style="border:0px;background-color:#005C00;width:32px;height:32px;color:#fff;text-align:center">$0A
</td>
<td style="border:0px;background-color:#005324;width:32px;height:32px;color:#fff;text-align:center">$0B
</td>
<td style="border:0px;background-color:#003C76;width:32px;height:32px;color:#fff;text-align:center">$0C
</td>
<td style="border:0px;background-color:#000000;width:32px;height:32px;color:#fff;text-align:center"><a href="/wiki/Color_$0D_games#Effects" title="Color $0D games"><s style="color:red">$0D</s></a>
</td>
<td style="border:0px;background-color:#000000;width:32px;height:32px;color:#fff;text-align:center">$0E
</td>
<td style="border:0px;background-color:#000000;width:32px;height:32px;color:#fff;text-align:center">$0F
</td></tr>
<tr>
<td style="border:0px;background-color:#ABABAB;width:32px;height:32px;color:#000;text-align:center">$10
</td>
<td style="border:0px;background-color:#0D57FF;width:32px;height:32px;color:#fff;text-align:center">$11
</td>
<td style="border:0px;background-color:#4B30FF;width:32px;height:32px;color:#fff;text-align:center">$12
</td>
<td style="border:0px;background-color:#8A13FF;width:32px;height:32px;color:#fff;text-align:center">$13
</td>
<td style="border:0px;background-color:#BC08D6;width:32px;height:32px;color:#fff;text-align:center">$14
</td>
<td style="border:0px;background-color:#D21269;width:32px;height:32px;color:#fff;text-align:center">$15
</td>
<td style="border:0px;background-color:#C72E00;width:32px;height:32px;color:#fff;text-align:center">$16
</td>
<td style="border:0px;background-color:#9D5400;width:32px;height:32px;color:#fff;text-align:center">$17
</td>
<td style="border:0px;background-color:#607B00;width:32px;height:32px;color:#fff;text-align:center">$18
</td>
<td style="border:0px;background-color:#209800;width:32px;height:32px;color:#fff;text-align:center">$19
</td>
<td style="border:0px;background-color:#00A300;width:32px;height:32px;color:#fff;text-align:center">$1A
</td>
<td style="border:0px;background-color:#009942;width:32px;height:32px;color:#fff;text-align:center">$1B
</td>
<td style="border:0px;background-color:#007DB4;width:32px;height:32px;color:#fff;text-align:center">$1C
</td>
<td style="border:0px;background-color:#000000;width:32px;height:32px;color:#fff;text-align:center">$1D
</td>
<td style="border:0px;background-color:#000000;width:32px;height:32px;color:#fff;text-align:center">$1E
</td>
<td style="border:0px;background-color:#000000;width:32px;height:32px;color:#fff;text-align:center">$1F
</td></tr>
<tr>
<td style="border:0px;background-color:#FFFFFF;width:32px;height:32px;color:#000;text-align:center">$20
</td>
<td style="border:0px;background-color:#53AEFF;width:32px;height:32px;color:#000;text-align:center">$21
</td>
<td style="border:0px;background-color:#9085FF;width:32px;height:32px;color:#000;text-align:center">$22
</td>
<td style="border:0px;background-color:#D365FF;width:32px;height:32px;color:#000;text-align:center">$23
</td>
<td style="border:0px;background-color:#FF57FF;width:32px;height:32px;color:#000;text-align:center">$24
</td>
<td style="border:0px;background-color:#FF5DCF;width:32px;height:32px;color:#000;text-align:center">$25
</td>
<td style="border:0px;background-color:#FF7757;width:32px;height:32px;color:#000;text-align:center">$26
</td>
<td style="border:0px;background-color:#FA9E00;width:32px;height:32px;color:#000;text-align:center">$27
</td>
<td style="border:0px;background-color:#BDC700;width:32px;height:32px;color:#000;text-align:center">$28
</td>
<td style="border:0px;background-color:#7AE700;width:32px;height:32px;color:#000;text-align:center">$29
</td>
<td style="border:0px;background-color:#43F611;width:32px;height:32px;color:#000;text-align:center">$2A
</td>
<td style="border:0px;background-color:#26EF7E;width:32px;height:32px;color:#000;text-align:center">$2B
</td>
<td style="border:0px;background-color:#2CD5F6;width:32px;height:32px;color:#000;text-align:center">$2C
</td>
<td style="border:0px;background-color:#4E4E4E;width:32px;height:32px;color:#fff;text-align:center">$2D
</td>
<td style="border:0px;background-color:#000000;width:32px;height:32px;color:#fff;text-align:center">$2E
</td>
<td style="border:0px;background-color:#000000;width:32px;height:32px;color:#fff;text-align:center">$2F
</td></tr>
<tr>
<td style="border:0px;background-color:#FFFFFF;width:32px;height:32px;color:#000;text-align:center">$30
</td>
<td style="border:0px;background-color:#B6E1FF;width:32px;height:32px;color:#000;text-align:center">$31
</td>
<td style="border:0px;background-color:#CED1FF;width:32px;height:32px;color:#000;text-align:center">$32
</td>
<td style="border:0px;background-color:#E9C3FF;width:32px;height:32px;color:#000;text-align:center">$33
</td>
<td style="border:0px;background-color:#FFBCFF;width:32px;height:32px;color:#000;text-align:center">$34
</td>
<td style="border:0px;background-color:#FFBDF4;width:32px;height:32px;color:#000;text-align:center">$35
</td>
<td style="border:0px;background-color:#FFC6C3;width:32px;height:32px;color:#000;text-align:center">$36
</td>
<td style="border:0px;background-color:#FFD59A;width:32px;height:32px;color:#000;text-align:center">$37
</td>
<td style="border:0px;background-color:#E9E681;width:32px;height:32px;color:#000;text-align:center">$38
</td>
<td style="border:0px;background-color:#CEF481;width:32px;height:32px;color:#000;text-align:center">$39
</td>
<td style="border:0px;background-color:#B6FB9A;width:32px;height:32px;color:#000;text-align:center">$3A
</td>
<td style="border:0px;background-color:#A9FAC3;width:32px;height:32px;color:#000;text-align:center">$3B
</td>
<td style="border:0px;background-color:#A9F0F4;width:32px;height:32px;color:#000;text-align:center">$3C
</td>
<td style="border:0px;background-color:#B8B8B8;width:32px;height:32px;color:#000;text-align:center">$3D
</td>
<td style="border:0px;background-color:#000000;width:32px;height:32px;color:#fff;text-align:center">$3E
</td>
<td style="border:0px;background-color:#000000;width:32px;height:32px;color:#fff;text-align:center">$3F
</td></tr></tbody>
"""

soup = BeautifulSoup(html, 'html.parser')

td_elements = soup.find_all('td')

color_array = []

for td in td_elements:
    style_attr = td.get('style')
    if style_attr:
        style_attrs = style_attr.split(';')
        for attr in style_attrs:
            if 'background-color' in attr:
                bg_color = attr.split(':')[1]
                color_array.append(bg_color.replace("#", "0x"))

# Generate C array
print("const char *color_array[] = {")
for color in color_array:
    print(f'    "{color}",')
print("};")
