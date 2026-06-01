#ifndef _SVG_H
#define _SVG_H

/**
 * Este módulo é responsável por fornecer as funções necessárias para criar e manipular arquivos no formato SVG (Scalable Vector Graphics),
 * que é um formato de arquivo gráfico vetorial amplamente utilizado para representar imagens bidimensionais.
 * 
 * As funções deste módulo permitem criar um arquivo SVG, desenhar formas geométricas (como retângulos, círculos e linhas) 
 * no arquivo SVG e fechar o arquivo SVG após a escrita.
 * 
 * As formas geométricas desenhadas no arquivo SVG podem ser personalizadas com propriedades como 
 * cor de preenchimento, cor da borda e largura da borda, permitindo a criação de gráficos vetoriais personalizados.
 * 
 * O módulo SVG é utilizado principalmente para gerar representações visuais dos dados processados a partir dos arquivos .geo e .qry, 
 * facilitando a visualização e análise dos dados geográficos e das operações realizadas sobre eles.
 * 
 * As funções deste módulo são projetadas para serem utilizadas em conjunto com as outras partes do programa, 
 * como o processamento dos arquivos .geo e .qry, para criar representações visuais dos dados geográficos e das operações realizadas sobre eles.
 */

/**
 * Esta função cria um arquivo .svg com as dimensões especificadas e retorna um ponteiro para o arquivo criado.
 * @param caminhoSvg Caminho completo do arquivo .svg a ser criado
 * @param largura    Largura do arquivo .svg
 * @param altura     Altura do arquivo .svg
 * @return           Ponteiro para o arquivo .svg criado. NULL em caso de erro
 */
FILE* criarSvg(char* caminhoSvg, double largura, double altura);
/**
 * Esta função desenha uma forma geométrica (retângulo, círculo ou linha) no arquivo .svg especificado, 
 * utilizando as propriedades fornecidas como parâmetros.
 * 
 * @param arqSvg    Ponteiro para o arquivo .svg
 * @param tipoForma Tipo da forma a ser desenhada
 * @param x         Coordenada x da posição da forma
 * @param y         Coordenada y da posição da forma
 * @param w         Largura da forma
 * @param h         Altura da forma
 * @param sw        Largura da borda da forma
 * @param cstrk     Cor da borda da forma
 * @param cfill     Cor de preenchimento da forma
 * @return          0 em caso de sucesso. -1 em caso de erro
 */
int desenharFormaSvg(FILE* arqSvg, char* tipoForma, double x, double y, double w, double h, char* sw, char* cstrk, char* cfill);
/**
 * Esta função fecha o arquivo .svg especificado, escrevendo a tag de fechamento do elemento <svg> e liberando os recursos associados ao arquivo.
 * @param arqSvg Ponteiro para o arquivo .svg a ser fechado
 * @return       0 em caso de sucesso. -1 em caso de erro
 */
int fecharSvg(FILE* arqSvg);
/*###############################################################################################*/


#endif