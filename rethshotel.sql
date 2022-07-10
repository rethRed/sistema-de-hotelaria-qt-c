-- phpMyAdmin SQL Dump
-- version 5.1.1
-- https://www.phpmyadmin.net/
--
-- Host: 10.129.76.12
-- Tempo de geração: 10/07/2022 às 08:20
-- Versão do servidor: 5.6.26-log
-- Versão do PHP: 8.0.15

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Banco de dados: `rethshotel`
--

-- --------------------------------------------------------

--
-- Estrutura para tabela `Caixa`
--

CREATE TABLE `Caixa` (
  `Id` int(11) NOT NULL,
  `IdHotel` int(11) NOT NULL,
  `DataEntrada` datetime DEFAULT NULL,
  `DataSaida` datetime DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Despejando dados para a tabela `Caixa`
--

INSERT INTO `Caixa` (`Id`, `IdHotel`, `DataEntrada`, `DataSaida`) VALUES
(1, 1, '2022-06-04 17:18:17', NULL);

-- --------------------------------------------------------

--
-- Estrutura para tabela `MetodoPagamento`
--

CREATE TABLE `MetodoPagamento` (
  `Pagamento` varchar(50) NOT NULL,
  `IdHotel` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Despejando dados para a tabela `MetodoPagamento`
--

INSERT INTO `MetodoPagamento` (`Pagamento`, `IdHotel`) VALUES
('Dinheiro', 1),
('Pix', 1),
('Cartão De Débito', 1),
('Cartão De Crédito', 1);

-- --------------------------------------------------------

--
-- Estrutura para tabela `PagamentoInfo`
--

CREATE TABLE `PagamentoInfo` (
  `IdQuartosReservados` int(11) NOT NULL,
  `SubTotal` double NOT NULL,
  `Taxa` double NOT NULL,
  `Desconto` double NOT NULL,
  `Total` double NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Despejando dados para a tabela `PagamentoInfo`
--

INSERT INTO `PagamentoInfo` (`IdQuartosReservados`, `SubTotal`, `Taxa`, `Desconto`, `Total`) VALUES
(26, 30, 5, 2, 33),
(27, 35, 8, 5, 38);

-- --------------------------------------------------------

--
-- Estrutura para tabela `Pagamentos`
--

CREATE TABLE `Pagamentos` (
  `IdQuartosReservados` int(11) NOT NULL,
  `Metodopagamento` varchar(50) DEFAULT NULL,
  `ValorPago` double NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Despejando dados para a tabela `Pagamentos`
--

INSERT INTO `Pagamentos` (`IdQuartosReservados`, `Metodopagamento`, `ValorPago`) VALUES
(22, 'Dinheiro', 50),
(24, 'Dinheiro', 100),
(25, 'Dinheiro', 100),
(26, 'Dinheiro', 50),
(27, 'Dinheiro', 30),
(27, 'Pix', 8);

-- --------------------------------------------------------

--
-- Estrutura para tabela `PerfilHotel`
--

CREATE TABLE `PerfilHotel` (
  `Id` int(11) NOT NULL,
  `Nome` varchar(50) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Despejando dados para a tabela `PerfilHotel`
--

INSERT INTO `PerfilHotel` (`Id`, `Nome`) VALUES
(1, 'RethsHotel');

-- --------------------------------------------------------

--
-- Estrutura para tabela `PreçoHora`
--

CREATE TABLE `PreçoHora` (
  `Id` int(11) NOT NULL,
  `IdTipoQuarto` varchar(50) NOT NULL,
  `Hora` int(11) NOT NULL,
  `Preco` double NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Despejando dados para a tabela `PreçoHora`
--

INSERT INTO `PreçoHora` (`Id`, `IdTipoQuarto`, `Hora`, `Preco`) VALUES
(1, 'Solteiro', 1, 30),
(2, 'Solteiro', 2, 45),
(3, 'Solteiro', 3, 60),
(4, 'Solteiro', 100, 99),
(5, 'Solteiro', 93, 96),
(6, 'Solteiro', 91, 89);

-- --------------------------------------------------------

--
-- Estrutura para tabela `ProdutosConsumo`
--

CREATE TABLE `ProdutosConsumo` (
  `Id` int(11) NOT NULL,
  `IdHotel` int(11) NOT NULL,
  `Descricao` varchar(50) NOT NULL,
  `Valor` double NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Despejando dados para a tabela `ProdutosConsumo`
--

INSERT INTO `ProdutosConsumo` (`Id`, `IdHotel`, `Descricao`, `Valor`) VALUES
(6, 1, 'batata', 5),
(7, 1, 'Coca Cola', 7),
(8, 1, 'toalha', 3);

-- --------------------------------------------------------

--
-- Estrutura para tabela `Quartos`
--

CREATE TABLE `Quartos` (
  `Id` int(11) NOT NULL,
  `IdHotel` int(11) NOT NULL,
  `NumQuarto` int(11) NOT NULL,
  `TipoQuarto` varchar(50) NOT NULL,
  `IdQuartosReservados` int(11) DEFAULT NULL,
  `Status` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Despejando dados para a tabela `Quartos`
--

INSERT INTO `Quartos` (`Id`, `IdHotel`, `NumQuarto`, `TipoQuarto`, `IdQuartosReservados`, `Status`) VALUES
(1, 1, 1, 'Solteiro', NULL, 1),
(2, 1, 2, 'Casal', 23, 3),
(3, 1, 3, 'Solteiro', 24, 3),
(4, 1, 4, 'Solteiro', 25, 3),
(5, 1, 5, 'Solteiro', 26, 3),
(6, 1, 6, 'Casal', NULL, 1),
(7, 1, 7, 'Casal', NULL, 1),
(8, 1, 8, 'Solteiro', NULL, 1),
(9, 1, 9, 'Casal', NULL, 1),
(10, 1, 10, 'Casal', NULL, 1),
(11, 1, 11, 'Casal', NULL, 1);

-- --------------------------------------------------------

--
-- Estrutura para tabela `QuartosReservados`
--

CREATE TABLE `QuartosReservados` (
  `Id` int(11) NOT NULL,
  `IdQuarto` int(11) NOT NULL,
  `IdUsuario` int(11) DEFAULT NULL,
  `IdCaixa` int(11) DEFAULT NULL,
  `HorarioEntrada` datetime DEFAULT NULL,
  `HorarioSaida` datetime DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Despejando dados para a tabela `QuartosReservados`
--

INSERT INTO `QuartosReservados` (`Id`, `IdQuarto`, `IdUsuario`, `IdCaixa`, `HorarioEntrada`, `HorarioSaida`) VALUES
(22, 1, 1, NULL, '2022-06-17 15:32:45', '2022-06-17 15:43:42'),
(23, 2, 1, NULL, '2022-06-17 15:46:20', '2022-06-17 15:47:27'),
(24, 3, 1, NULL, '2022-06-17 15:48:18', '2022-06-17 15:48:20'),
(25, 4, 1, NULL, '2022-06-17 15:49:06', '2022-06-17 15:49:09'),
(26, 5, 1, NULL, '2022-06-17 15:57:43', '2022-06-17 15:57:47'),
(27, 1, 1, NULL, '2022-06-17 17:44:14', '2022-06-17 17:44:30');

-- --------------------------------------------------------

--
-- Estrutura para tabela `QuartosReservadosInfo`
--

CREATE TABLE `QuartosReservadosInfo` (
  `IdQuartosReservados` int(11) NOT NULL,
  `obs` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Despejando dados para a tabela `QuartosReservadosInfo`
--

INSERT INTO `QuartosReservadosInfo` (`IdQuartosReservados`, `obs`) VALUES
(22, ''),
(23, 'batata boa\n'),
(24, ''),
(25, ''),
(26, 'mas oque?'),
(27, 'batat\n');

-- --------------------------------------------------------

--
-- Estrutura para tabela `QuartoStatus`
--

CREATE TABLE `QuartoStatus` (
  `Id` int(11) NOT NULL,
  `Status` varchar(50) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Despejando dados para a tabela `QuartoStatus`
--

INSERT INTO `QuartoStatus` (`Id`, `Status`) VALUES
(1, 'Disponivel'),
(2, 'Ocupado'),
(3, 'Limpando'),
(4, 'Interditado'),
(5, 'Fechado');

-- --------------------------------------------------------

--
-- Estrutura para tabela `RegistroConsumo`
--

CREATE TABLE `RegistroConsumo` (
  `Id` int(11) NOT NULL,
  `IdCaixa` int(11) NOT NULL,
  `IdQuartosReservado` int(11) DEFAULT NULL,
  `IdUsuario` int(11) NOT NULL,
  `IdProduto` int(11) DEFAULT NULL,
  `DataResgistro` datetime NOT NULL,
  `Descricao` varchar(50) NOT NULL,
  `Qtd` int(11) NOT NULL,
  `Valor` double NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Despejando dados para a tabela `RegistroConsumo`
--

INSERT INTO `RegistroConsumo` (`Id`, `IdCaixa`, `IdQuartosReservado`, `IdUsuario`, `IdProduto`, `DataResgistro`, `Descricao`, `Qtd`, `Valor`) VALUES
(33, 1, 27, 1, 6, '2022-06-17 17:44:23', 'batata', 1, 5);

-- --------------------------------------------------------

--
-- Estrutura para tabela `TipoQuarto`
--

CREATE TABLE `TipoQuarto` (
  `Descricao` varchar(50) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Despejando dados para a tabela `TipoQuarto`
--

INSERT INTO `TipoQuarto` (`Descricao`) VALUES
('Casal'),
('Solteiro'),
('Suite');

-- --------------------------------------------------------

--
-- Estrutura para tabela `Usuario`
--

CREATE TABLE `Usuario` (
  `Id` int(11) NOT NULL,
  `IdHotel` int(11) NOT NULL,
  `Nome` varchar(50) NOT NULL,
  `Senha` varchar(30) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Despejando dados para a tabela `Usuario`
--

INSERT INTO `Usuario` (`Id`, `IdHotel`, `Nome`, `Senha`) VALUES
(1, 1, 'Sudo User', '123');

--
-- Índices para tabelas despejadas
--

--
-- Índices de tabela `Caixa`
--
ALTER TABLE `Caixa`
  ADD PRIMARY KEY (`Id`),
  ADD KEY `IdHotel` (`IdHotel`);

--
-- Índices de tabela `MetodoPagamento`
--
ALTER TABLE `MetodoPagamento`
  ADD KEY `IdHotel` (`IdHotel`);

--
-- Índices de tabela `PagamentoInfo`
--
ALTER TABLE `PagamentoInfo`
  ADD KEY `IdQuartosReservados` (`IdQuartosReservados`);

--
-- Índices de tabela `Pagamentos`
--
ALTER TABLE `Pagamentos`
  ADD KEY `IdQuartosReservados` (`IdQuartosReservados`);

--
-- Índices de tabela `PerfilHotel`
--
ALTER TABLE `PerfilHotel`
  ADD PRIMARY KEY (`Id`);

--
-- Índices de tabela `PreçoHora`
--
ALTER TABLE `PreçoHora`
  ADD PRIMARY KEY (`Id`),
  ADD KEY `IdTipoQuarto` (`IdTipoQuarto`);

--
-- Índices de tabela `ProdutosConsumo`
--
ALTER TABLE `ProdutosConsumo`
  ADD PRIMARY KEY (`Id`),
  ADD KEY `IdHotel` (`IdHotel`);

--
-- Índices de tabela `Quartos`
--
ALTER TABLE `Quartos`
  ADD PRIMARY KEY (`Id`),
  ADD KEY `IdHotel` (`IdHotel`),
  ADD KEY `Status` (`Status`),
  ADD KEY `Quartos_ibfk_4` (`IdQuartosReservados`),
  ADD KEY `TipoQuarto` (`TipoQuarto`);

--
-- Índices de tabela `QuartosReservados`
--
ALTER TABLE `QuartosReservados`
  ADD PRIMARY KEY (`Id`),
  ADD KEY `IdUsuario` (`IdUsuario`),
  ADD KEY `IdCaixa` (`IdCaixa`);

--
-- Índices de tabela `QuartosReservadosInfo`
--
ALTER TABLE `QuartosReservadosInfo`
  ADD PRIMARY KEY (`IdQuartosReservados`);

--
-- Índices de tabela `QuartoStatus`
--
ALTER TABLE `QuartoStatus`
  ADD PRIMARY KEY (`Id`);

--
-- Índices de tabela `RegistroConsumo`
--
ALTER TABLE `RegistroConsumo`
  ADD PRIMARY KEY (`Id`),
  ADD KEY `IdCaixa` (`IdCaixa`),
  ADD KEY `IdProduto` (`IdProduto`),
  ADD KEY `IdQuarto` (`IdQuartosReservado`),
  ADD KEY `IdUsuario` (`IdUsuario`);

--
-- Índices de tabela `TipoQuarto`
--
ALTER TABLE `TipoQuarto`
  ADD PRIMARY KEY (`Descricao`);

--
-- Índices de tabela `Usuario`
--
ALTER TABLE `Usuario`
  ADD PRIMARY KEY (`Id`),
  ADD KEY `IdHotel` (`IdHotel`);

--
-- AUTO_INCREMENT para tabelas despejadas
--

--
-- AUTO_INCREMENT de tabela `Caixa`
--
ALTER TABLE `Caixa`
  MODIFY `Id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT de tabela `PerfilHotel`
--
ALTER TABLE `PerfilHotel`
  MODIFY `Id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT de tabela `PreçoHora`
--
ALTER TABLE `PreçoHora`
  MODIFY `Id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;

--
-- AUTO_INCREMENT de tabela `ProdutosConsumo`
--
ALTER TABLE `ProdutosConsumo`
  MODIFY `Id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=9;

--
-- AUTO_INCREMENT de tabela `Quartos`
--
ALTER TABLE `Quartos`
  MODIFY `Id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=12;

--
-- AUTO_INCREMENT de tabela `QuartosReservados`
--
ALTER TABLE `QuartosReservados`
  MODIFY `Id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=28;

--
-- AUTO_INCREMENT de tabela `QuartoStatus`
--
ALTER TABLE `QuartoStatus`
  MODIFY `Id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;

--
-- AUTO_INCREMENT de tabela `RegistroConsumo`
--
ALTER TABLE `RegistroConsumo`
  MODIFY `Id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=34;

--
-- AUTO_INCREMENT de tabela `Usuario`
--
ALTER TABLE `Usuario`
  MODIFY `Id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- Restrições para tabelas despejadas
--

--
-- Restrições para tabelas `Caixa`
--
ALTER TABLE `Caixa`
  ADD CONSTRAINT `Caixa_ibfk_1` FOREIGN KEY (`IdHotel`) REFERENCES `PerfilHotel` (`Id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Restrições para tabelas `MetodoPagamento`
--
ALTER TABLE `MetodoPagamento`
  ADD CONSTRAINT `MetodoPagamento_ibfk_1` FOREIGN KEY (`IdHotel`) REFERENCES `PerfilHotel` (`Id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Restrições para tabelas `PagamentoInfo`
--
ALTER TABLE `PagamentoInfo`
  ADD CONSTRAINT `PagamentoInfo_ibfk_1` FOREIGN KEY (`IdQuartosReservados`) REFERENCES `QuartosReservados` (`Id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Restrições para tabelas `Pagamentos`
--
ALTER TABLE `Pagamentos`
  ADD CONSTRAINT `Pagamentos_ibfk_1` FOREIGN KEY (`IdQuartosReservados`) REFERENCES `QuartosReservados` (`Id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Restrições para tabelas `PreçoHora`
--
ALTER TABLE `PreçoHora`
  ADD CONSTRAINT `PreçoHora_ibfk_1` FOREIGN KEY (`IdTipoQuarto`) REFERENCES `TipoQuarto` (`Descricao`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Restrições para tabelas `ProdutosConsumo`
--
ALTER TABLE `ProdutosConsumo`
  ADD CONSTRAINT `ProdutosConsumo_ibfk_1` FOREIGN KEY (`IdHotel`) REFERENCES `PerfilHotel` (`Id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Restrições para tabelas `Quartos`
--
ALTER TABLE `Quartos`
  ADD CONSTRAINT `Quartos_ibfk_2` FOREIGN KEY (`IdHotel`) REFERENCES `PerfilHotel` (`Id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `Quartos_ibfk_3` FOREIGN KEY (`Status`) REFERENCES `QuartoStatus` (`Id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `Quartos_ibfk_4` FOREIGN KEY (`IdQuartosReservados`) REFERENCES `QuartosReservados` (`Id`) ON DELETE SET NULL ON UPDATE NO ACTION,
  ADD CONSTRAINT `Quartos_ibfk_5` FOREIGN KEY (`TipoQuarto`) REFERENCES `TipoQuarto` (`Descricao`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Restrições para tabelas `QuartosReservados`
--
ALTER TABLE `QuartosReservados`
  ADD CONSTRAINT `QuartosReservados_ibfk_1` FOREIGN KEY (`IdUsuario`) REFERENCES `Usuario` (`Id`) ON DELETE SET NULL ON UPDATE CASCADE,
  ADD CONSTRAINT `QuartosReservados_ibfk_2` FOREIGN KEY (`IdCaixa`) REFERENCES `Caixa` (`Id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Restrições para tabelas `QuartosReservadosInfo`
--
ALTER TABLE `QuartosReservadosInfo`
  ADD CONSTRAINT `QuartosReservadosInfo_ibfk_1` FOREIGN KEY (`IdQuartosReservados`) REFERENCES `QuartosReservados` (`Id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Restrições para tabelas `RegistroConsumo`
--
ALTER TABLE `RegistroConsumo`
  ADD CONSTRAINT `RegistroConsumo_ibfk_1` FOREIGN KEY (`IdCaixa`) REFERENCES `Caixa` (`Id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `RegistroConsumo_ibfk_2` FOREIGN KEY (`IdProduto`) REFERENCES `ProdutosConsumo` (`Id`) ON DELETE SET NULL ON UPDATE CASCADE,
  ADD CONSTRAINT `RegistroConsumo_ibfk_3` FOREIGN KEY (`IdQuartosReservado`) REFERENCES `QuartosReservados` (`Id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `RegistroConsumo_ibfk_4` FOREIGN KEY (`IdUsuario`) REFERENCES `Usuario` (`Id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Restrições para tabelas `Usuario`
--
ALTER TABLE `Usuario`
  ADD CONSTRAINT `Usuario_ibfk_1` FOREIGN KEY (`IdHotel`) REFERENCES `PerfilHotel` (`Id`) ON DELETE CASCADE ON UPDATE CASCADE;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
