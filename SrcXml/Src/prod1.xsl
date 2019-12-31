<?xml version="1.0"?>
<xsl:stylesheet 
      xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
      version="1.0">
   <xsl:template match="/titulo">
       <HTML>
         <HEAD>
           <TITLE></TITLE>
            <style>
                 H1{FONT-SIZE: 200%; COLOR: #448a06}
				 .desc{FONT-SIZE: 200%; COLOR: #448a06}
            </style>		   
         </HEAD>
         <BODY>
            <xsl:apply-templates select="titulo"/>
         </BODY>
       </HTML>
   </xsl:template>
   <xsl:template match="titulo">
        <DIV> 
           <xsl:value-of select="."/>
       </DIV>
   </xsl:template>

      
</xsl:stylesheet>