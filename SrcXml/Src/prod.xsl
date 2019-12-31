<?xml version="1.0"?>
<xsl:stylesheet 
      xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
      version="1.0">
   <xsl:template match="/menuitem">
       <HTML>
         <HEAD>
           <TITLE></TITLE>
            <style>
                 H1{FONT-SIZE: 200%; COLOR: #448a06}
				 .desc{FONT-SIZE: 200%; COLOR: #448a06}
            </style>		   
         </HEAD>
         <BODY>
            <H1 style="FONT-SIZE: 200%; COLOR: #448a06"><xsl:value-of select="label"/></H1>
            <p class="desc"><xsl:apply-templates select="titulo"/></p>
			<xsl:apply-templates select="img"/>
			<xsl:apply-templates select="submenu"/>
			
         </BODY>
       </HTML>
   </xsl:template>
   <xsl:template match="submenu">
       <DIV> 
           <p><I><xsl:value-of select="label"/></I></p>
		   <xsl:apply-templates select="select"/>
		   <xsl:apply-templates select="input"/>
		   
       </DIV>
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
  <xsl:attribute name="font-size">12pt</xsl:attribute>
  <xsl:attribute name="font-weight">bold</xsl:attribute>
</xsl:attribute-set>

      
</xsl:stylesheet>