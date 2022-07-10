
from flask import Flask,jsonify,request,g
import json
import mysql.connector 
import requests


class BancoDeDados:

    def __init__(self):
        self.dataBase = None
        self.cursor = None

    def conectarBancoDeDados(self):
        self.dataBase = mysql.connector.connect(
            host = "",
            user="",
            passwd="",
            database=""
        )
        self.cursor = self.dataBase.cursor(dictionary=True)

    def fecharBancoDeDados(self):

        if self.dataBase != None:
            self.dataBase.close()


app = Flask(__name__)

@app.before_request
def abrirBancoDeDados():
    g.database = BancoDeDados()
    g.database.conectarBancoDeDados()

@app.teardown_request
def fecharBancoDeDados(x):
    g.database.fecharBancoDeDados()


@app.route("/api/GetAllRooms",methods=["POST"])
def getAllRooms():
    
    database = g.database
    
    userRequest = request.get_json()
    if not jsonHasKeys(userRequest,["hotelId"]):
        return {}

    HotelId = userRequest["hotelId"] 


    database.cursor.execute(f'''
    select Quartos.Id Id,LPAD(Quartos.numQuarto, 3, '0') numQuarto , QuartoStatus.Status Status,Quartos.TipoQuarto Tipo ,
    Quartos.IdQuartosReservados IdReserva
    from Quartos 
    left join QuartoStatus on QuartoStatus.Id = Quartos.Status
    where Quartos.IdHotel = %s
    order by Quartos.NumQuarto 
    ''',(HotelId,))

    resultado = database.cursor.fetchall()

    return jsonify(resultado)


@app.route("/api/HospedarQuarto",methods=["POST"])
def hospedarQuarto():
    database = g.database
    
    userRequest = request.get_json()

    keysNeeded = ["hotelId","idQuarto","obs","Id_usuario"]
    if not jsonHasKeys(userRequest,keysNeeded):
        return {"Status": "Rejeitado","message":""}

    #adiciona quartos reservados
    database.cursor.execute("""
    INSERT INTO `QuartosReservados`  
    (`IdQuarto`, `IdUsuario`, `HorarioEntrada`) 
    VALUES ( %s, %s, Now())
    """,(userRequest["idQuarto"],userRequest["Id_usuario"]))

    database.cursor.execute("SELECT ifnull(LAST_INSERT_ID(),0) QuartosReservadosId;")
    QuartosReservados_id = database.cursor.fetchall()
    if not QuartosReservados_id:
        menssage = "Erro ao cadastrar a grade."
        return {"menssage":menssage,"status":"Rejeitado"}
    QuartosReservados_id = QuartosReservados_id[0]["QuartosReservadosId"]

    #adicionar info 
    database.cursor.execute("""
        INSERT INTO `QuartosReservadosInfo` (`IdQuartosReservados`, `obs`) VALUES (%s, %s)
    """,(QuartosReservados_id,userRequest["obs"]))

    #seta o id do quartos reservados no quartos
    database.cursor.execute("""
        Update Quartos set IdQuartosReservados = %s, Status = 2 where id = %s
    """,(QuartosReservados_id,userRequest["idQuarto"]))
    
    database.dataBase.commit()

    return {"Status": "Sucesso","Reserva_id":str(QuartosReservados_id)}


@app.route("/api/GetConsumos",methods=["POST"])

def getConsumos():
    database = g.database
    userRequest = request.get_json()
    keysNeeded = ["hotelId"]

    if not jsonHasKeys(userRequest,keysNeeded):
        return {"Status": "Rejeitado","message":""}

    database.cursor.execute("""
        select Id,Descricao,replace(TRUNCATE(Valor,2),".",",") Preco from ProdutosConsumo where IdHotel = %s
    """,(userRequest["hotelId"],))
    
    return jsonify(database.cursor.fetchall())


@app.route("/api/ResgistrarConsumos",methods=["POST"])
def addConsumos():
    database = g.database
    
    userRequest = request.get_json()

    keysNeeded = ["Id_usuario","id_caixa","id_produto","id_reserva"]
    if not jsonHasKeys(userRequest,keysNeeded):
        return {"Status": "Rejeitado","message":""}

    database.cursor.execute("select Descricao,Qtd,Valor from RegistroConsumo where idProduto = %s and IdQuartosReservado = %s ",
    (userRequest["id_produto"],userRequest["id_reserva"]))

    resposta = database.cursor.fetchall()
    
    if not resposta:

        database.cursor.execute("select Descricao, valor from ProdutosConsumo where id = %s",(userRequest["id_produto"],))

        produtoInfo = database.cursor.fetchone()
        if not produtoInfo:
            return {}

        database.cursor.execute("""
            INSERT INTO `RegistroConsumo` 
            ( `IdCaixa`, `IdQuartosReservado`, `IdUsuario`, `IdProduto`, `DataResgistro`, `Descricao`, `Qtd`, `Valor`) 
            VALUES ( %s, %s, %s, %s, Now(), %s, %s, %s)
            """,(userRequest["id_caixa"],userRequest["id_reserva"],userRequest["Id_usuario"]
        ,userRequest["id_produto"],produtoInfo["Descricao"],1,produtoInfo["valor"]))

        database.dataBase.commit()

        database.cursor.execute("SELECT ifnull(LAST_INSERT_ID(),0) RegistroId;")

        RegistroId = database.cursor.fetchone()
        if not RegistroId:
            return {"status":0}

        database.cursor.execute("""
        select RegistroConsumo.Qtd qtd, replace(TRUNCATE(RegistroConsumo.Valor,2),".",",") precoTotal,
        replace(TRUNCATE(pd.Valor,2),".",",") precoUn,pd.Descricao descricao
        from RegistroConsumo 
        left join ProdutosConsumo pd 
        on pd.id = RegistroConsumo.IdProduto
        where RegistroConsumo.Id = %s
        """,(RegistroId["RegistroId"],))

        produtoInfo = database.cursor.fetchone()
        if not produtoInfo:
            return {}

        return {"Status":"Produto nao Existente","descricao":produtoInfo["descricao"],
        "precoUn":produtoInfo["precoUn"],"precoTotal":produtoInfo["precoTotal"],"Qtd":produtoInfo["qtd"],"IdRegistro":RegistroId["RegistroId"]}


    else:
        print("else")
        database.cursor.execute("select Id from RegistroConsumo where idProduto = %s and IdQuartosReservado = %s "
        ,(userRequest["id_produto"],userRequest["id_reserva"]))

        produtoInfo = database.cursor.fetchone()
        if not produtoInfo:
            return {}
        database.cursor.execute("""
        update RegistroConsumo left join ProdutosConsumo on  ProdutosConsumo.Id = RegistroConsumo.IdProduto 
        set RegistroConsumo.Qtd = RegistroConsumo.Qtd+1 , RegistroConsumo.Valor = RegistroConsumo.Qtd*ProdutosConsumo.Valor
        where RegistroConsumo.Id = %s 
        """,(produtoInfo["Id"],))

        database.dataBase.commit()

        database.cursor.execute("""
        select RegistroConsumo.Qtd qtd, replace(TRUNCATE(RegistroConsumo.Valor,2),".",",") precoTotal,
        replace(TRUNCATE(pd.Valor,2),".",",") precoUn,pd.Descricao descricao
        from RegistroConsumo 
        left join ProdutosConsumo pd 
        on pd.id = RegistroConsumo.IdProduto
        where RegistroConsumo.Id = %s
        """,(produtoInfo["Id"],))

        registroId = produtoInfo["Id"]
        produtoInfo = database.cursor.fetchone()
        if not produtoInfo:
            return {}
        

        return {"Status":"Produto ja Existente","descricao":produtoInfo["descricao"],
        "precoUn":produtoInfo["precoUn"],"precoTotal":produtoInfo["precoTotal"],"Qtd":produtoInfo["qtd"],"IdRegistro":registroId}


@app.route("/api/DeletarConsumos",methods=["POST"])
def DeletarConsumo():
    database = g.database
    userRequest = request.get_json()

    keysNeeded = ["hotelId","ConsumoRegistroId"]
    if not jsonHasKeys(userRequest,keysNeeded):
        return {"Status": "Rejeitado","message":""}

    database.cursor.execute("""
    delete from RegistroConsumo where Id = %s
    """,(userRequest["ConsumoRegistroId"],))

    database.dataBase.commit()

    return {"Status": "Deletado Com Sucesso"}


@app.route("/api/informacoesDoQuarto",methods=["POST"])
def getRoomInformation():
    database = g.database
    userRequest = request.get_json()

    keysNeeded = ["hotelId","quartoId","reservaId"]
    if not jsonHasKeys(userRequest,keysNeeded):
        return {"Status": "Rejeitado","message":""}

    database.cursor.execute("""
    select DATE_FORMAT(qr.HorarioEntrada,"%d/%m/%Y %T") HorarioEntrada, ifnull(qri.obs,"") obs
    from QuartosReservados qr
    left join QuartosReservadosInfo qri on qr.id = qri.IdQuartosReservados
    where qr.id = %s
    """,(userRequest["reservaId"],))

    reservaInfo = database.cursor.fetchone()

    database.cursor.execute("""

        select rc.Qtd , replace(TRUNCATE(rc.Valor,2),".",",") precoTotal, 
        pc.Descricao Descricao, replace(TRUNCATE(pc.Valor,2),".",",") precoUn,rc.Id IdRegistro
        from RegistroConsumo rc
        join ProdutosConsumo pc on rc.IdProduto = pc.Id
        where rc.IdQuartosReservado = %s

    """,(userRequest["reservaId"],))

    consumoInfo = database.cursor.fetchall()


    return {"ReservaInfo":reservaInfo,"ConsumoInfo":consumoInfo}

@app.route("/api/getCheckoutInfo",methods=["POST"])
def checkoutInfo():

    database = g.database
    userRequest = request.get_json()

    keysNeeded = ["hotelId","reservaId"]
    if not jsonHasKeys(userRequest,keysNeeded):
        return {"Status": "Rejeitado","message":""}

    database.cursor.execute("""
    
SELECT tp.HorarioEntrada,tp.HorarioSaida,CAST(now() as CHAR(22)) horarioSaidaNotFormated,
CAST(tp.tempoPermanecido as CHAR(9)) tempoPermanecido,
ifnull(TRUNCATE(sum(rc.Valor),2),0.00) TotalConsumo,

ifnull((
    SELECT ifnull(TRUNCATE(Preco,2),0.00)  from PreçoHora 
    where PreçoHora.IdTipoQuarto = Quartos.TipoQuarto 
    order by  abs( HOUR(tp.tempoPermanecido) - PreçoHora.Hora) 
    limit 1
),0.00) ValorSugerido


from Quartos
join(
   	SELECT DATE_FORMAT(qr.HorarioEntrada,"%d/%m/%Y %T") HorarioEntrada, 
	DATE_FORMAT(now(),"%d/%m/%Y %T") HorarioSaida,
	SEC_TO_TIME(TIMESTAMPDIFF(Second,qr.HorarioEntrada ,Now())) tempoPermanecido,
    qr.IdQuarto,
    qr.id
    from QuartosReservados qr 
    where qr.Id = %s
	) tp
join RegistroConsumo rc on rc.IdQuartosReservado = tp.id

where Quartos.Id = tp.IdQuarto

    """,(userRequest["reservaId"],))

    reservaInfo = database.cursor.fetchone()

    reservaInfo["Status"] = "Enviado Com Sucesso"

    database.cursor.execute("""
        select Pagamento 
        from MetodoPagamento
        where IdHotel = %s
    """,(userRequest["hotelId"],))

    reservaInfo["MetodoPagamento"] = database.cursor.fetchall()

    return reservaInfo


@app.route("/api/completarCheckout",methods=["POST"])
def fazerCheckout():
    database = g.database
    userRequest = request.get_json()

    keysNeeded = ["hotelId","tempoSaida","reservaId","Total","listaPagamentos","taxa","desconto","subTotal"]
    if not jsonHasKeys(userRequest,keysNeeded):
        return {"Status": "Rejeitado","message":""}

    database.cursor.execute("""
        INSERT INTO `PagamentoInfo` 
        (`IdQuartosReservados`, `SubTotal`, `Taxa`, `Desconto`, `Total`) 
        VALUES (%s, %s, %s, %s, %s)
    """,(userRequest["reservaId"],userRequest["subTotal"],userRequest["taxa"],
        userRequest["desconto"],userRequest["Total"]))

    database.cursor.execute("""
        update QuartosReservados qr 
        left join Quartos on Quartos.IdQuartosReservados = qr.Id
        set qr.HorarioSaida = %s,Quartos.Status = 3
        where qr.Id = %s
    """,(userRequest["tempoSaida"],userRequest["reservaId"]))

    pagamentos = userRequest["listaPagamentos"]

    for pagamentoKey in pagamentos:
        
        valorPago = pagamentos[pagamentoKey]

        database.cursor.execute("""
            INSERT INTO `Pagamentos` 
            (`IdQuartosReservados`, `Metodopagamento`, `ValorPago`) 
            VALUES (%s, %s, %s)
        """,(userRequest["reservaId"],pagamentoKey,valorPago))

    database.dataBase.commit()
    return{"Status":"Sucesso"}

@app.route("/api/abrirQuarto",methods=["POST"])
def abrirQuarto():
    database = g.database
    userRequest = request.get_json()

    keysNeeded = ["hotelId","QuartoId"]
    if not jsonHasKeys(userRequest,keysNeeded):
        return {"Status": "Rejeitado","message":""}
    
    database.cursor.execute("""
        update Quartos 
        set IdQuartosReservados = null, Status = 1
        where id = %s  

    """,(userRequest["QuartoId"],))

    database.dataBase.commit()

    return {"Status": "Sucesso"}


def jsonHasKeys(requestJson,keyNames):
    for name in keyNames:
        if name not in requestJson:
            return False
    return True

app.run(host="localhost")