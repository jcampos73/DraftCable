<?xml version="1.0"?>
<xsl:stylesheet 
      xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
      version="1.0">
   <xsl:template match="/catalog">
       <HTML>
         <HEAD>
           <TITLE></TITLE>
            <style>
                 H1{FONT-SIZE: 200%; COLOR: #448a06}
				 .desc{FONT-SIZE: 200%; COLOR: #448a06}
            </style>
	<STYLE TYPE="text/css">
	     p.breakhere {page-break-before: always}
	</STYLE>             
	<style>
	<!--
	 /* Style Definitions */
	p.MsoNormal, li.MsoNormal, div.MsoNormal
		{mso-style-parent:"";
		margin:0cm;
		margin-bottom:.0001pt;
		mso-pagination:widow-orphan;
		font-size:12.0pt;
		font-family:"Times New Roman";
		mso-fareast-font-family:"Times New Roman";}
	p.MsoTitle, li.MsoTitle, div.MsoTitle
		{margin:0cm;
		margin-bottom:.0001pt;
		text-align:center;
		mso-pagination:widow-orphan;
		font-size:12.0pt;
		mso-bidi-font-size:10.0pt;
		font-family:"Times New Roman";
		mso-fareast-font-family:"Times New Roman";
		mso-bidi-language:HE;
		font-weight:bold;
		mso-bidi-font-weight:normal;}
	@page Section1
		{size:595.3pt 841.9pt;
		margin:70.85pt 3.0cm 70.85pt 3.0cm;
		mso-header-margin:36.0pt;
		mso-footer-margin:36.0pt;
		mso-paper-source:0;}
	div.Section1
		{page:Section1;}
	-->
	</style>            		   
         </HEAD>
         <BODY>

            <xsl:apply-templates select="cable"/>
			
		</BODY>
	</HTML>
	</xsl:template>
   
	<xsl:template match="cable">
		<table>
		<xsl:attribute name="border">1</xsl:attribute>
			<tr style='height:10.0pt'>
				<td><xsl:attribute name="colspan">2</xsl:attribute>
				Company name
				</td>
				<td><xsl:attribute name="colspan">2</xsl:attribute>
				OBRA(REF):
				</td>
				<xsl:apply-templates select="cable_num"/>
				<td><xsl:attribute name="colspan">2</xsl:attribute>
				HOJA  DE 
				</td>
			</tr>
			<tr style='height:9.9pt'>
				<xsl:apply-templates select="cpar"/>
				<xsl:apply-templates select="clongi"/>
				<xsl:apply-templates select="cable_tipo"/>
			</tr>		
			<tr style='height:9.9pt'>
				<xsl:apply-templates select="de_equipo"/>
				<xsl:apply-templates select="a_equipo"/>
			</tr>
			<tr style='height:9.9pt'>
				<xsl:apply-templates select="de_situadoen"/>
				<xsl:apply-templates select="a_situadoen"/>
			</tr>
			<tr style='height:9.9pt'>
				<xsl:apply-templates select="de_conectortipo"/>
				<xsl:apply-templates select="a_conectortipo"/>
			</tr>
			<tr>
				<td><xsl:attribute name="colspan">3</xsl:attribute>
				DE
				</td>
				<td><xsl:attribute name="colspan">2</xsl:attribute>
				SERVICIO
				</td>
				<td><xsl:attribute name="colspan">2</xsl:attribute>
				A
				</td>
			</tr>
			<tr>
				<td>
				P/J
				</td>
				<td>
				PIN
				</td>
				<td>
				REF.HILO
				</td>
				<td>
				FUNCION O NOMBRE DE LA SENAL
				</td>
				<td>
				P/J
				</td>
				<td>
				PIN
				</td>
			</tr>
			<xsl:apply-templates select="wire"/>
		</table>   
		
		<xsl:text>&#x0a;</xsl:text>
		
		<p><xsl:attribute name="class">breakhere</xsl:attribute></p>
	    
	</xsl:template>
   
	<xsl:template match="p">
		<xsl:copy>
		<xsl:attribute name="class">breakhere</xsl:attribute>
		<xsl:attribute name="src"><xsl:value-of select="."/></xsl:attribute>
		</xsl:copy>   
	</xsl:template>


   <xsl:template match="cable_num">
		<td>
		CABLE(N<sup>O</sup>):<xsl:value-of select="."/>
		</td> 
   </xsl:template>
   <xsl:template match="cpar">
		<td>
		<xsl:attribute name="colspan">2</xsl:attribute>
		CABLE(N<sup>O</sup>P):<xsl:value-of select="."/>
		</td> 
   </xsl:template>
   <xsl:template match="clongi">
		<td>
		<xsl:attribute name="colspan">2</xsl:attribute>
		CABLE(LONGITUD):<xsl:value-of select="."/>
		</td> 
   </xsl:template>
   <xsl:template match="cable_tipo">
		<td>
		<xsl:attribute name="colspan">3</xsl:attribute>
		CABLE(TIPO):<xsl:value-of select="."/>
		</td> 
   </xsl:template>
   <xsl:template match="de_equipo">
		<td><xsl:attribute name="colspan">4</xsl:attribute>
		DE/EQUIPO:<xsl:value-of select="."/>
		</td> 
   </xsl:template>
   <xsl:template match="a_equipo">
		<td><xsl:attribute name="colspan">3</xsl:attribute>
		A/EQUIPO:<xsl:value-of select="."/>
		</td> 
   </xsl:template>   
   <xsl:template match="de_situadoen">
		<td><xsl:attribute name="colspan">4</xsl:attribute>
		DE/SITUADO EN:<xsl:value-of select="."/>
		</td> 
   </xsl:template>  
   <xsl:template match="a_situadoen">
		<td><xsl:attribute name="colspan">3</xsl:attribute>
		A/SITUADO EN:<xsl:value-of select="."/>
		</td> 
   </xsl:template>     
   <xsl:template match="de_conectortipo">
		<td><xsl:attribute name="colspan">4</xsl:attribute>
		DE/CONECTOR(TIPO):<xsl:value-of select="."/>
		</td> 
   </xsl:template>  
   <xsl:template match="a_conectortipo">
		<td><xsl:attribute name="colspan">3</xsl:attribute>
		A/CONECTOR(TIPO):<xsl:value-of select="."/>
		</td> 
   </xsl:template>     
   <xsl:template match="pj_de">
		<td>
		<xsl:value-of select="."/>
		</td> 
   </xsl:template>     
   <xsl:template match="pin_de">
		<td>
		<xsl:value-of select="."/>
		</td> 
   </xsl:template>     
   <xsl:template match="ref_hilo">
		<td>
		<xsl:value-of select="."/>
		</td> 
   </xsl:template>     
   <xsl:template match="funcion">
		<td>
		<xsl:value-of select="."/>
		</td> 
   </xsl:template>     
   <xsl:template match="pj_a">
		<td>
		<xsl:value-of select="."/>
		</td> 
   </xsl:template>        
   <xsl:template match="pin_a">
		<td>
		<xsl:value-of select="."/>
		</td> 
   </xsl:template>     
   <xsl:template match="wire">
		<tr>
			<xsl:apply-templates select="pj_de"/>
			<xsl:apply-templates select="pin_de"/>
			<xsl:apply-templates select="ref_hilo"/>
			<xsl:apply-templates select="funcion"/>
			<xsl:apply-templates select="pj_a"/>
			<xsl:apply-templates select="pin_a"/>
		</tr>
   </xsl:template>     

         
   <xsl:template match="titulo">
        <DIV> 
           <xsl:value-of select="."/>
       </DIV>
   </xsl:template>
   
	<xsl:template match="img">
		<xsl:copy>
			<xsl:attribute name="style">FLOAT: right</xsl:attribute>
			<xsl:attribute name="width">150</xsl:attribute>
			<xsl:attribute name="height">108</xsl:attribute>
			<xsl:attribute name="src"><xsl:value-of select="."/></xsl:attribute>
		</xsl:copy>
	</xsl:template>
   
	<xsl:template match="select">
		<xsl:copy>
			<xsl:attribute name="name"><xsl:value-of select="@name"/></xsl:attribute>
			<xsl:attribute name="onchange">debugsel(this)</xsl:attribute>
			<xsl:apply-templates select="option"/>
		</xsl:copy>
	</xsl:template>
   
	<xsl:template match="option">
		<xsl:copy>
			<xsl:attribute name="value"><xsl:value-of select="@value"/></xsl:attribute>
			<xsl:value-of select="."/>
		</xsl:copy>
	</xsl:template>   
   
	<xsl:template match="input">
		<xsl:copy>
			<xsl:attribute name="type"><xsl:value-of select="@type"/></xsl:attribute>
			<xsl:attribute name="name"><xsl:value-of select="@name"/></xsl:attribute>
		  	<xsl:attribute name="value"><xsl:value-of select="label"/></xsl:attribute>
			<xsl:attribute name="onclick">debugbut(this)</xsl:attribute>
		</xsl:copy>
	</xsl:template>
   
	<xsl:attribute-set name="title-style">
		<xsl:attribute name="border">1</xsl:attribute>
		<xsl:attribute name="font-size">12pt</xsl:attribute>
		<xsl:attribute name="font-weight">bold</xsl:attribute>
	</xsl:attribute-set>
      
</xsl:stylesheet>