# Pong Clone
<h1>Clone de pong usando c++ e SFML</h1>

<p>
    Esse jogo foi feito em c++ usando a biblioteca SFML. <br>
    A arquitetura do jogo é feito em ECS (Entity-Component-System). <br>
    É possível, através do arquivo de texto config.txt, mudar diversas configurações do jogo, como o tamanho e cores da raquete e bola, e mais. <br>
</p>
<img width="1282" height="752" alt="image" src="https://github.com/user-attachments/assets/d0c8c866-f503-4bb6-b47f-fcaa0797c363" />
<img width="1282" height="752" alt="image" src="https://github.com/user-attachments/assets/1aadafcf-1ce4-4df0-bcbe-e73099b10d90" />
<img width="1282" height="752" alt="image" src="https://github.com/user-attachments/assets/70bcf294-3869-46dd-8928-153d58904987" />


<h2>Instruções:</h2>

<p>Menu inicial: pressione '1' para um jogador e '2' para dois jogadores.</p>

<p>Controles para um jogador: W e seta para cima movimenta para cima, e S e seta para baixo movimenta para baixo</p>

<p>Controles para dois jogadores: W e S para a raquete da esquerda, setas para cima e baixo para a raquete da direita.</p>

<p>Na tela final: M para voltar ao menu inicial, R para recomeçar no mesmo modo de jogo.</p>

<br>

<h2>Significado do config.txt:</h2>

<h3>Window [W, H, FL, FS]:</h3>
<ul>
    <li>W - Largura da Janela</li>
    <li>H - Altura da Janela</li>
    <li>FL - Limite de Framerate</li>
    <li>FS - Se vai começar em tela cheia ou não</li>
</ul>

<h3>Player [W, H, FR, FG, FB, OR, OG, OB, OT, S]</h3>
<ul>
    <li>W - Largura do player</li>
    <li>H - Altura do player</li>
    <li>FR, FG, FB - Cor de preenchimento do player</li>
    <li>OR, OG, OB - Cor de contorno</li>
    <li>OT - Tamanho do contorno</li>
    <li>S - Velocidade</li>
</ul>

<h3>Ball [SR, FR, FG, FB, OR, OG, OB, OT, V, IS]</h3>
<ul>
    <li>SR - Raio da bola</li>
    <li>FR, FG, FB - Cor de preenchimento da bola</li>
    <li>OR, OG, OB - Cor de contorno</li>
    <li>OT - Tamanho do contorno</li>
    <li>V - Quantidade de vértices da bola</li>
    <li>IS - Velocidade inicial</li>
</ul>


<h3>Score [MS]</h3>
<ul>
    <li>MS - Quantidade de pontos para acabar o jogo</li>
</ul>

<h3>Valores padrão</h3>
Window 1280 720 60 0 <br>
Font fonts\font.ttf 48 255 255 255<br>
Player 40 180 255 255 255 0 0 0 0 10<br>
Ball 24 255 255 255 0 0 0 0 32 8<br>
Score 3 <br>
<br>
Fonte usada: https://www.dafont.com/vcr-osd-mono.font


