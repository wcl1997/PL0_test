/*
(1)���丳ֵ���㣺*= �� /= �� <>(������)
(2)������䣨Pascal��FOR��䣩: 
    1)FOR <����>:=<���ʽ> TO <���ʽ> DO <���> 
    2)FOR <����>:=<���ʽ> DOWNTO <���ʽ> DO <���> 
    ���У����1��ѭ�������Ĳ���Ϊ1�� 
    ���2��ѭ�������Ĳ���Ϊ-1�� 
��3���������ͣ��ַ����ͣ� 
 (4) ���� ע��; ע����/*��/������ 
��5���������㣺++ �� --��
��6��else ����

*ʹ�÷���:
*���к�����PL/0 Դ�����ļ���
*�ش��Ƿ�������������
*�ش��Ƿ�������ֱ�
*fa.tmp ������������
*fa1.tmp  ���Դ�ļ�������ж�Ӧ���׵�ַ
*fa2.tmp  ������ 
*fas.tmp  ������ֱ�
*/
#include <stdio.h>
#include "pl0.h"
#include "string.h"
/*����ִ��ʱʹ�õ�ջ*/
#define stacksize 500
int main()
{
    bool nxtlev[symnum];
    printf("Input pl/0 file ?");
    scanf("%s", fname); /*�����ļ���*/
    fin = fopen(fname, "r");
    if (fin)
    {
        printf("List object code ?(Y/N)"); /*�Ƿ�������������*/
        scanf("%s", fname);
        listswitch = (fname[0] == 'y' || fname[0] == 'Y');
        printf("List symbol table ? (Y/N)"); /*�Ƿ�������ֱ�*/
        scanf("%s", fname);
        tableswitch = (fname[0] == 'y' || fname[0] == 'Y');
        fa1 = fopen("fa1.tmp", "w");
        fprintf(fa1, "Iput pl/0 file ?");
        fprintf(fa1, "%s\n", fname);
        init(); /*��ʼ��*/
        err = 0;
        cc = cx = ll = 0;
        ch = ' ';
        if (-1 != getsym())
        {
            fa = fopen("fa.tmp", "w");
            fas = fopen("fas.tmp", "w");
            addset(nxtlev, declbegsys, statbegsys, symnum);
            nxtlev[period] = true;
            if (-1 == block(0, 0, nxtlev)) /*���ñ������*/
            {
                fclose(fa);
                fclose(fa1);
                fclose(fas);
                fclose(fin);
                printf("\n");
                return 0;
            }
            fclose(fa);
            fclose(fa1);
            fclose(fas);
            if (sym != period)
            {
                error(9);
            }
            if (err == 0)
            {
                fa2 = fopen("fa2.tmp", "w");
                interpret();
                fclose(fa2);
            }
            else
            {
                printf("Errors in pl/0 program");
            }
        }
        fclose(fin);
    }
    else
    {
        printf("Can't open file! \n");
    }
    printf("\n");
    getchar();
    getchar();
    return 0;
}
/*
*��ʼ��
*/
void init()
{
    int i;
    for (i = 0; i <= 255; i++)
    {
        ssym[i] = nul;
    }
    ssym['+'] = plus;
    ssym['-'] = minus;
    ssym['*'] = times;
    ssym['/'] = slash;
    ssym['('] = lparen;
    ssym[')'] = rparen;
    ssym['='] = eql;
    ssym[','] = comma;
    ssym['.'] = period;
    // ssym['#']=neq;  //ȥ��ԭ�ȵ��ַ���#�������ò���
    ssym[';'] = semicolon;
    /*���ñ���������,������ĸ˳��,�����۰����*/
    strcpy(&(word[0][0]), "begin");
    strcpy(&(word[1][0]), "call");
    strcpy(&(word[2][0]), "char"); /*���ӱ�����char*/
    strcpy(&(word[3][0]), "const");
    strcpy(&(word[4][0]), "do");
    strcpy(&(word[5][0]), "downto"); /*���ӱ�����downto*/
    strcpy(&(word[6][0]), "else");   /*���ӱ�����else*/
    strcpy(&(word[7][0]), "end");
    strcpy(&(word[8][0]), "for"); /**���ӱ�����for*/
    strcpy(&(word[9][0]), "if");
    strcpy(&(word[10][0]), "odd");
    strcpy(&(word[11][0]), "procedure");
    strcpy(&(word[12][0]), "read");
    strcpy(&(word[13][0]), "return"); /*���ӱ�����return*/
    strcpy(&(word[14][0]), "then");
    strcpy(&(word[15][0]), "to"); /*���ӱ�����to*/
    strcpy(&(word[16][0]), "var");
    strcpy(&(word[17][0]), "while");
    strcpy(&(word[18][0]), "write");
    /*���ñ����ַ���*/
    wsym[0] = beginsym;
    wsym[1] = callsym;
    wsym[2] = charsym; /*���ӱ����ַ���charsym*/
    wsym[3] = constsym;
    wsym[4] = dosym;
    wsym[5] = downtosym; /*���ӱ����ַ���downtosym*/
    wsym[6] = elsesym;   /*���ӱ����ַ���elsesym*/
    wsym[7] = endsym;
    wsym[8] = forsym; /*���ӱ����ַ���forsym*/
    wsym[9] = ifsym;
    wsym[10] = oddsym;
    wsym[11] = procsym;
    wsym[12] = readsym;
    wsym[13] = returnsym; /*���ӱ����ַ���returnsym*/
    wsym[14] = thensym;
    wsym[15] = tosym; /*���ӱ����ַ���tosym*/
    wsym[16] = varsym;
    wsym[17] = whilesym;
    wsym[18] = writesym;

    /*����ָ������*/
    strcpy(&(mnemonic[lit][0]), "lit");
    strcpy(&(mnemonic[opr][0]), "opr");
    strcpy(&(mnemonic[lod][0]), "lod");
    strcpy(&(mnemonic[sto][0]), "sto");
    strcpy(&(mnemonic[cal][0]), "cal");
    strcpy(&(mnemonic[inte][0]), "int");
    strcpy(&(mnemonic[jmp][0]), "jmp");
    strcpy(&(mnemonic[jpc][0]), "jpc");

    /*���÷��ż�*/
    for (i = 0; i < symnum; i++)
    {
        declbegsys[i] = false;
        statbegsys[i] = false;
        facbegsys[i] = false;
    }

    /*����������ʼ���ż�*/
    declbegsys[constsym] = true;
    declbegsys[varsym] = true;
    declbegsys[procsym] = true;
    declbegsys[charsym] = true; //����������ʼ����charsym

    /*������俪ʼ���ż�*/
    statbegsys[beginsym] = true;
    statbegsys[callsym] = true;
    statbegsys[ifsym] = true;
    statbegsys[whilesym] = true;

    /*�������ӿ�ʼ���ż�*/
    facbegsys[ident] = true;
    facbegsys[number] = true;
    facbegsys[lparen] = true;
    facbegsys[plusplus] = true;   //���ӿ�ʼ����plusplus
    facbegsys[minusminus] = true; //���ӿ�ʼ����minusminus
    facbegsys[charsym] = true;    //���ӿ�ʼ����charsym
}
 /*
  *������ʵ�ּ��ϵļ�������
  */
int inset(int e, bool *s)
{
    return s[e];
}
int addset(bool *sr, bool *s1, bool *s2, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        sr[i] = s1[i] || s2[i];
    }
    return 0;
}
int subset(bool *sr, bool *s1, bool *s2, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        sr[i] = s1[i] && (!s2[i]);
    }
    return 0;
}
int mulset(bool *sr, bool *s1, bool *s2, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        sr[i] = s1[i] && s2[i];
    }
    return 0;
}
/*
 *��������ӡ����λ�úʹ������
 */
void error(int n)
{
    char space[81];
    memset(space, 32, 81);
    printf("-------%c\n", ch);
    space[cc - 1] = 0; //����ʱ��ǰ�����Ѿ����꣬����cc-1
    printf("****%s!%d\n", space, n);
    err++;
}
/*
 *  ©���ո񣬶�ȡһ���ַ�
 *  ÿ�ζ�һ�У�����line��������line��getsymȡ�պ��ٶ�һ��
 *  ������getsym����
 */
int getch()
{
    if (cc == ll)
    {
        if (feof(fin))
        {
            printf("program incomplete");
            return -1;
        }
        ll = 0;
        cc = 0;
        printf("%d ", cx);
        fprintf(fa1, "%d ", cx);
        ch = ' ';
        while (ch != 10)
        {
            //fscanf(fin,"%c",&ch)
            if (EOF == fscanf(fin, "%c", &ch))
            {
                line[ll] = 0;
                break;
            }
            printf("%c", ch);
            fprintf(fa1, "%c", ch);
            line[ll] = ch;
            ll++;
        }
        printf("\n");
        fprintf(fa1, "\n");
    }
    ch = line[cc];
    cc++;
    return 0;
}

/*�ʷ���������ȡһ������
 */
int getsym()
{
    int i, j, k;
    while (ch == ' ' || ch == 10 || ch == 9)
    {
        getchdo;
    }
    if (ch >= 'a' && ch <= 'z')
    {
        k = 0;
        do
        {
            if (k < al)
            {
                a[k] = ch;
                k++;
            }
            getchdo;
        } while (ch >= 'a' && ch <= 'z' || ch >= '0' && ch <= '9');
        a[k] = 0;
        strcpy(id, a);
        i = 0;
        j = norw - 1;
        do
        {
            k = (i + j) / 2;
            if (strcmp(id, word[k]) <= 0)
            {
                j = k - 1;
            }
            if (strcmp(id, word[k]) >= 0)
            {
                i = k + 1;
            }

        } while (i <= j);
        if (i - 1 > j)
        {
            sym = wsym[k];
        }
        else
        {
            sym = ident;
        }
    }
    else
    {
        if (ch >= '0' && ch <= '9')
        {
            k = 0;
            num = 0;
            sym = number;
            do
            {
                num = 10 * num + ch - '0';
                k++;
                getchdo;
            } while (ch >= '0' && ch <= '9'); /*��ȡ���ֵ�ֵ*/
            k--;
            if (k > nmax)
            {
                error(30);
            }
        }
        else
        {
            if (ch == ':') /*��⸳ֵ����*/
            {
                getchdo;
                if (ch == '=')
                {
                    sym = becomes;
                    getchdo;
                }
                else
                {
                    sym = nul; /*����ʶ��ķ���*/
                }
            }
            else
            {
                if (ch == '<') /*���С�ڻ�С�ڵ��ڷ���*/
                {
                    getchdo;
                    if (ch == '=')
                    {
                        sym = leq;
                        getchdo;
                    }
                    //���Ӳ��Ⱥ��﷨����
                    else if (ch == '>') //С�ںź�����Ŵ��ں�
                    {
                        sym = neq; //���ɲ��Ⱥ�<>
                        printf("ʶ���<>��");
                        getchdo;
                    }
                    else
                    {
                        sym = lss;
                    }
                }
                else
                {
                    if (ch == '>') /*�����ڻ���ڵ��ڷ���*/
                    {
                        getchdo;
                        if (ch == '=')
                        {
                            sym = geq;
                            getchdo;
                        }
                        else
                        {
                            sym = gtr;
                        }
                    }
                    else if (ch == '*')
                    {
                        getchdo;
                        if (ch == '=')
                        {
                            sym = timeseql; //����*=��
                            printf("ʶ���*=��\n");
                            getchdo;
                        }
                        else
                        {
                            sym = times;
                        }
                    }
                    else if (ch == '/')
                    {
                        getchdo;
                        if (ch == '=')
                        {
                            sym = slasheql; //����/=��
                            printf("ʶ���/=��\n");
                            getchdo;
                        }
                        else if (ch == '*') //����ע�͹���
                        {
                            int end = 1;
                            char a = 0, b = 0;
                            getchdo;
                            printf("ʶ���ע��/*%c", ch);
                            while (end)
                            {
                                if (b != 0)
                                {
                                    b = a;
                                }
                                else
                                    b = ch;
                                getchdo;
                                a = ch;

                                printf("%c", a);
                                if (b == '*' && a == '/')
                                {
                                    printf("ע�͵��˽���\n");
                                    end = 0;
                                    break;
                                }
                            }
                            getchdo;
                        }
                        else
                        {
                            sym = slash;
                        }
                    }
                    else if (ch == '+')
                    {
                        getchdo;
                        if (ch == '+')
                        {
                            sym = plusplus; //����++��
                            printf("ʶ���++��\n");
                            getchdo;
                        }
                        else
                        {
                            sym = plus;
                        }
                    }
                    else if (ch == '-')
                    {
                        getchdo;
                        if (ch == '-')
                        {
                            sym = minusminus; //����--��
                            printf("ʶ���--��\n");
                            getchdo;
                        }
                        else
                        {
                            sym = minus;
                        }
                    }
                    else if (ch == '\'') //�����ַ���
                    {
                        getchdo;
                        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
                        {
                            num = (int)ch;
                            getchdo;
                            if (ch == '\'')
                                sym = charsym;
                            else
                            {
                                num = 0;
                                sym = nul;
                                error(39); //���ʹ���
                            }
                        }
                        else
                            error(39); //���Ͳ�ƥ��
                        getchdo;
                    }
                    else
                    {
                        sym = ssym[ch]; /* �����Ų�������������ʱ��ȫ�����յ��ַ��Ŵ���*/
                        //getchdo;
                        //richard
                        if (sym != period)
                        {
                            getchdo;
                        }
                        //end richard
                    }
                }
            }
        }
    }
    return 0;
}
/*
*�������������
*
*x:instruction.f;
*y:instruction.l;
*z:instruction.a;
*/
int gen(enum fct x, int y, int z)
{
    if (cx >= cxmax)
    {
        printf("Program too long"); /*�������*/
        return -1;
    }
    code[cx].f = x;
    code[cx].l = y;
    code[cx].a = z;
    cx++;
    return 0;
}

/*
*���Ե�ǰ�����Ƿ�Ϸ�
*
*��ĳһ���֣���һ����䣬һ�����ʽ����Ҫ����ʱʱ����ϣ����һ����������ĳ����
*���ò��ֵĺ�����ţ� test ���������⣬���Ҹ��𵱼�ⲻͨ��ʱ�Ĳ��ȴ�ʩ
*��������Ҫ���ʱָ����ǰ��Ҫ�ķ��ż��ϺͲ����õļ��ϣ���֮ǰδ��ɲ��ֵĺ��
*���ţ����Լ���ͨ��ʱ�Ĵ����
*
*S1��������Ҫ�ķ���
*s2:�������������Ҫ�ģ�����Ҫһ�������õļ���
*n:�����
*/
int test(bool *s1, bool *s2, int n)
{
    if (!inset(sym, s1))
    {
        error(n);
        /*����ⲻͨ��ʱ����ͣ��ȡ���ţ�ֱ����������Ҫ�ļ��ϻ򲹾ȵļ���*/
        while ((!inset(sym, s1)) && (!inset(sym, s2)))
        {
            getsymdo;
        }
    }
    return 0;
}
/*
 *�����������
 *
 *lev:��ǰ�ֳ������ڲ�
 *tx:���ֱ�ǰβָ��
 *fsys:��ǰģ�������ż���
 */
int block(int lev, int tx, bool *fsys)
{
    int i;
    int dx;              /*���ַ��䵽����Ե�ַ*/
    int tx0;             /*������ʼtx*/
    int cx0;             /*������ʼcx*/
    bool nxtlev[symnum]; /*���¼������Ĳ����У����ż��Ͼ�Ϊֵ�Σ�������ʹ������
                                      ʵ�֣����ݽ�������ָ�룬Ϊ��ֹ�¼������ı��ϼ�������
                                      ���ϣ������µĿռ䴫�ݸ��¼�����*/
    dx = 3;
    tx0 = tx; /*��¼�������ֵĳ�ʼλ��*/
    table[tx].adr = cx;
    gendo(jmp, 0, 0);
    if (lev > levmax)
    {
        error(32);
    }
    do
    {
        if (sym == constsym) /*�յ������������ţ���ʼ����������*/
        {
            getsymdo;
            do
            {
                constdeclarationdo(&tx, lev, &dx); /*dx��ֵ�ᱻconstdeclaration�ı䣬ʹ��
                                                     ָ��*/
                while (sym == comma)
                {
                    getsymdo;
                    constdeclarationdo(&tx, lev, &dx);
                }
                if (sym == semicolon)
                {
                    getsymdo;
                }
                else
                {
                    error(5); /*©���˶��Ż��߷ֺ�*/
                }
            } while (sym == ident);
        }
        if (sym == varsym) /*�յ������������ţ���ʼ�����������*/
        {
            getsymdo;
            do
            {
                vardeclarationdo(&tx, lev, &dx);
                while (sym == comma)
                {
                    getsymdo;
                    vardeclarationdo(&tx, lev, &dx);
                }
                if (sym == semicolon)
                {
                    getsymdo;
                }
                else
                {
                    error(5);
                }
            } while (sym == ident);
        }
        if (sym == charsym) /*�յ��������������ţ���ʼ�������������*/
        {
            getsymdo;
            do
            {
                chardeclarationdo(&tx, lev, &dx);
                while (sym == comma)
                {
                    getsymdo;
                    chardeclarationdo(&tx, lev, &dx);
                }
                if (sym == semicolon)
                {
                    getsymdo;
                }
                else
                {
                    error(5);
                }
            } while (sym == ident);
        }
        while (sym == procsym) /*�յ������������ţ���ʼ�����������*/
        {
            getsymdo;
            if (sym == ident)
            {
                enter(procedur, &tx, lev, &dx); /*��¼��������*/
                getsymdo;
            }
            else
            {
                error(4); /*procedure��ӦΪ��ʶ��*/
            }
            if (sym == semicolon)
            {
                getsymdo;
            }
            else
            {
                error(5); /*©���˷ֺ�*/
            }
            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
            nxtlev[semicolon] = true;
            if (-1 == block(lev + 1, tx, nxtlev))
            {
                return -1; /*�ݹ����*/
            }
            if (sym == semicolon)
            {
                getsymdo;
                memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
                nxtlev[ident] = true;
                nxtlev[procsym] = true;
                testdo(nxtlev, fsys, 6);
            }
            else
            {
                error(5); /*©���˷ֺ�*/
            }
        }
        memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
        nxtlev[ident] = true;
        nxtlev[period] = true;
        testdo(nxtlev, declbegsys, 7);
    } while (inset(sym, declbegsys)); /*ֱ��û����������*/
    code[table[tx0].adr].a = cx;      /*��ʼ���ɵ�ǰ���̴���*/
    table[tx0].adr = cx;              /*��ǰ���̴����ַ*/
    table[tx0].size = dx;             /*����������ÿ����һ�����������dx����1,���������Ѿ�����,dx���ǵ�ǰ�������ݵ�size*/

    cx0 = cx;
    gendo(inte, 0, dx); /*���ɷ����ڴ����*/
    if (tableswitch)    /*������ֱ�*/
    {
        printf("TABLE:\n");
        if (tx0 + 1 > tx)
        {
            printf("NULL\n");
        }
        for (i = tx0 + 1; i <= tx; i++)
        {
            switch (table[i].kind)
            {
            case constant:
                printf("%d const %s", i, table[i].name);
                printf("val=%d\n", table[i].val);
                fprintf(fas, "%d const %s", i, table[i].name);
                fprintf(fas, "val=%d\n", table[i].val);
                break;
            case variable:
                printf("%d var%s", i, table[i].name);
                printf("lev=%d addr=%d\n", table[i].level, table[i].adr);
                fprintf(fas, "%d var %s", i, table[i].name);
                fprintf(fas, "lev=%d addr=%d\n", table[i].level, table[i].adr);
                break;
            case procedur:
                printf("%d proc%s", i, table[i].name);
                printf("lev=%d addr=%d size=%d\n", table[i].level, table[i].adr, table[i].size);
                fprintf(fas, "%d proc%s", i, table[i].name);
                fprintf(fas, "lev=%d adr=%d size=%d \n", table[i].level, table[i].adr, table[i].size);
                break;
            }
        }
        printf("\n");
    }
    /*���������Ϊ�ֺŻ�end*/
    memcpy(nxtlev, fsys, sizeof(bool) * symnum); /*ÿ��������ż��Ͷ������ϲ������ż��ͣ��Ա㲹��*/
    nxtlev[semicolon] = true;
    nxtlev[endsym] = true;
    statementdo(nxtlev, &tx, lev);
    gendo(opr, 0, 0);                         /*ÿ�����̳��ڶ�Ҫʹ�õ��ͷ����ݶ�����*/
    memset(nxtlev, 0, sizeof(bool) * symnum); /*�ֳ���û�в��ȼ���*/
    test(fsys, nxtlev, 8);                    /*�����������ȷ��*/
    listcode(cx0);                            /*�������*/
    return 0;
}
/*
*�����ֱ��м���һ��
*
*k:��������const,var or procedure
*ptx:���ֱ�βָ���ָ�룬Ϊ�˿��Ըı����ֱ�βָ�����ֵ
*lev:�������ڵĲ�Σ��Ժ����е�lev��������
*pdx:Ϊ��ǰӦ����ı�������Ե�ַ�������Ҫ����1
*/
void enter(enum object k, int *ptx, int lev, int *pdx)
{
    (*ptx)++;
    strcpy(table[(*ptx)].name, id); /*ȫ�ֱ���id���Ѵ��е�ǰ���ֵ�����*/
    table[(*ptx)].kind = k;
    switch (k)
    {
    case constant: /*��������*/
        if (num > amax)
        {
            error(31);
            num = 0;
        }
        table[(*ptx)].val = num;
        break;
    case variable: /*��������*/
        table[(*ptx)].level = lev;
        table[(*ptx)].adr = (*pdx);
        (*pdx)++;
        break; /*��������*/
    case procedur:
        table[(*ptx)].level = lev;
        break;
    case charcon: /*�ַ�������*/
        table[(*ptx)].level = lev;
        table[(*ptx)].adr = (*pdx);
        (*pdx)++;
        break;
    }
}
/*
 *�������ֵ�λ��
 *�ҵ��򷵻������ֱ��е�λ�ã����򷵻�0
 *
 *idt: Ҫ���ҵ�����
 *tx:����ǰ���ֱ�βָ��
 */
int position(char *idt, int tx)
{
    int i;
    strcpy(table[0].name, idt);
    i = tx;
    while (strcmp(table[i].name, idt) != 0)
    {
        i--;
    }
    return i;
}
/*
 *������������
 */
int constdeclaration(int *ptx, int lev, int *pdx)
{
    if (sym == ident)
    {
        getsymdo;
        if (sym == eql || sym == becomes)
        {
            if (sym == becomes)
            {
                error(1); /*��=д�����ˣ�=*/
            }
            getsymdo;
            if (sym == number)
            {
                enter(constant, ptx, lev, pdx);
                getsymdo;
            }
            else
            {
                error(2); /*����˵��=��Ӧ������*/
            }
        }
        else
        {
            error(3); /*����˵����ʶ��Ӧ��=*/
        }
    }
    else
    {
        error(4); /*const��Ӧ�Ǳ�ʶ*/
    }
    return 0;
}
/*
 *������������
 */
int vardeclaration(int *ptx, int lev, int *pdx)
{
    if (sym == ident)
    {
        enter(variable, ptx, lev, pdx); //��д���ֱ�
        getsymdo;
    }
    else
    {
        error(4);
    }
    return 0;
}
/*
 *�ַ�������(�޸�)
 */
int chardeclaration(int *ptx, int lev, int *pdx)
{
    if (sym == ident)
    {
        enter(charcon, ptx, lev, pdx); //��д���ֱ�
        getsymdo;
    }
    else
    {
        error(4);
    }
    return 0;
}
/*
 *ʵ������
 */
/*int realdeclaration(int * ptx,int lev,int * pdx)
{
    if(sym==ident)
    {
        enter(realcon,ptx,lev,pdx);//��д���ֱ�
        getsymdo;
    }
    else
    {
        error(4);
    }
    return 0;
}
*/

/*
  *����Ŀ������嵥
  */
void listcode(int cx0)
{
    int i;
    if (listswitch)
    {
        for (i = cx0; i < cx; i++)
        {
            printf("%d %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
            fprintf(fa, "%d %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
        }
    }
}
/*
*��䴦��
*/
int statement(bool *fsys, int *ptx, int lev)
{
    int i, cx1, cx2;
    bool nxtlev[symnum];
    if (sym == ident)
    {
        i = position(id, *ptx); //�ڷ��ű��в鵽�ñ�ʶ������λ��
        if (i == 0)
        {
            error(11);
        }
        else
        {
            if ((table[i].kind != variable) && (table[i].kind != charcon))
            {
                error(12);
                i = 0;
            }
            else if (table[i].kind == charcon)
            {
                getsymdo;
                if (sym != becomes)
                    error(13);
                else
                {
                    getsymdo;
                    if (sym != charsym)
                        error(32);
                    else
                    {
                        gendo(lit, 0, num);
                        gendo(sto, lev - table[i].level, table[i].adr);
                    }
                }
                getsymdo; //�ӣ��ֺ�
            }
            else
            {
                getsymdo;
                if (sym == becomes)
                {
                    getsymdo;
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    expressiondo(nxtlev, ptx, lev);
                    if (i != 0) //�����������i����Ϊ0��i��ָΪ��ǰ���
                                //�󲿱�ʶ���ڷ��ű��е�λ��
                    {
                        gendo(sto, lev - table[i].level, table[i].adr);
                    }
                }
                else if (sym == timeseql) // *=����
                {
                    getsymdo;
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    gendo(lod, lev - table[i].level, table[i].adr);
                    expressiondo(nxtlev, ptx, lev);
                    if (i != 0)
                    {
                        gendo(opr, 0, 4);
                        gendo(sto, lev - table[i].level, table[i].adr);
                    }
                }
                else if (sym == slasheql) // /=����
                {
                    getsymdo;
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    gendo(lod, lev - table[i].level, table[i].adr);
                    expressiondo(nxtlev, ptx, lev);
                    if (i != 0)
                    {
                        gendo(opr, 0, 5);
                        gendo(sto, lev - table[i].level, table[i].adr);
                    }
                }
                else if (sym == plusplus) // ��++����
                {
                    getsymdo;
                    gendo(lod, lev - table[i].level, table[i].adr); //�ҵ�������ַ������ֵ��ջ
                    gendo(lit, 0, 1);                               //������1ȡ��ջ��
                    if (i != 0)
                    {
                        gendo(opr, 0, 2); //ִ�мӲ���
                        gendo(sto, lev - table[i].level, table[i].adr);
                    }
                }
                else if (sym == minusminus) // ��--����
                {
                    getsymdo;
                    gendo(lod, lev - table[i].level, table[i].adr); //�ҵ�������ַ������ֵ��ջ
                    gendo(lit, 0, 1);                               //������1ȡ��ջ��
                    if (i != 0)
                    {
                        gendo(opr, 0, 3); //ִ�м�����
                        gendo(sto, lev - table[i].level, table[i].adr);
                    }
                }
                else
                {
                    error(13);
                }
            }
        }
    }
    else if (sym == plusplus) // ǰ++����
    {
        getsymdo;
        if (sym == ident)
        {
            i = position(id, *ptx);
            if (i == 0)
                error(11);
            else
            {
                if (table[i].kind != variable)
                {
                    error(12);
                    i = 0;
                }
                else //++������Ǳ���
                {
                    getsymdo;
                    gendo(lod, lev - table[i].level, table[i].adr); //�ҵ�������ַ������ֵ��ջ
                    gendo(lit, 0, 1);                               //������1ȡ��ջ��
                    if (i != 0)
                    {
                        gendo(opr, 0, 2); //ִ�мӲ���
                        gendo(sto, lev - table[i].level, table[i].adr);
                    }
                }
            }
        }
        else
        {
            error(19);
        }
    }
    else if (sym == minusminus) // ǰ--����
    {
        getsymdo;
        if (sym == ident)
        {
            i = position(id, *ptx);
            if (i == 0)
                error(11);
            else
            {
                if (table[i].kind != variable)
                {
                    error(12);
                    i = 0;
                }
                else //--������Ǳ���
                {
                    getsymdo;
                    gendo(lod, lev - table[i].level, table[i].adr); //�ҵ�������ַ������ֵ��ջ
                    gendo(lit, 0, 1);                               //������1ȡ��ջ��
                    if (i != 0)
                    {
                        gendo(opr, 0, 3); //ִ�м�����
                        gendo(sto, lev - table[i].level, table[i].adr);
                    }
                }
            }
        }
        else
        {
            error(19);
        }
    }
    else if (sym == forsym) //��⵽for���
    {
        getsymdo;
        if (sym == ident)
        {
            i = position(id, *ptx);
            if (i == 0)
                error(11);
            else
            {
                if (table[i].kind != variable) //��ֵ����У���ֵ���󲿱�ʶ������Ӧ�Ǳ���
                {
                    error(12);
                    i = 0;
                }
                else
                {
                    getsymdo;
                    if (sym != becomes)
                        error(13); //��ֵ����󲿱�ʶ����Ӧ�Ǹ�ֵ��:=
                    else
                        getsymdo;
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    nxtlev[tosym] = true; //�����to��downto
                    nxtlev[downtosym] = true;
                    expressiondo(nxtlev, ptx, lev);                 //����ֵ����Ҳ��ı��ʽE1
                    gendo(sto, lev - table[i].level, table[i].adr); //�����ֵ
                    switch (sym)
                    {
                    case tosym: //����Ϊ����������
                        getsymdo;
                        cx1 = cx; //����ѭ����ʼ��
                        //��ѭ���жϱ���ȡ���ŵ�ջ��
                        gendo(lod, lev - table[i].level, table[i].adr);
                        memcpy(nxtlev, fsys, sizeof(bool) * symnum); //������ʽE2
                        nxtlev[dosym] = true;                        //�����do
                        expressiondo(nxtlev, ptx, lev);
                        /*�ж�ѭ����������������for i:=E1 to E2 do S�У��ж�i�Ƿ�С��E2����С�ڵ��ڣ�����ѭ�������ڵĻ�������ѭ��*/
                        gendo(opr, 0, 13); //���ɱȽ�ָ�i�Ƿ�С�ڵ���E2��ֵ
                        cx2 = cx;          //����ѭ��������
                        //����������תָ�����ѭ���������ĵ�ַδ֪
                        gendo(jpc, 0, 0);
                        if (sym == dosym) //����ѭ����S
                        {
                            getsymdo;
                            statement(fsys, ptx, lev); //ѭ���崦��
                            //����ѭ����������Ϊ
                            //��ѭ������ȡ������ջ��
                            gendo(lod, lev - table[i].level, table[i].adr);
                            gendo(lit, 0, 1); //������ȡ��ջ��
                            gendo(opr, 0, 2); //ѭ�������Ӳ���
                            //��ջ����ֵ����ѭ������
                            gendo(sto, lev - table[i].level, table[i].adr);
                            gendo(jmp, 0, cx1); //��������ת��ѭ����ʼ��
                            /*����ѭ��������ĵ�ַ��cxΪelse�����ִ�����λ�ã�������ǰ��δ������ת��ַ*/
                            code[cx2].a = cx;
                        }
                        else
                        {
                            error(29); //for���������do
                        }
                        break;

                    case downtosym: //����Ϊ�����¼���
                        getsymdo;
                        cx1 = cx; //����ѭ����ʼ��
                        //��ѭ���жϱ���ȡ���ŵ�ջ��
                        gendo(lod, lev - table[i].level, table[i].adr);
                        memcpy(nxtlev, fsys, sizeof(bool) * symnum); //������ʽE2
                        nxtlev[dosym] = true;                        //�����do
                        expressiondo(nxtlev, ptx, lev);
                        /*�ж�ѭ����������������for i:=E1 downto E2 do S�У��ж�i�Ƿ���ڵ���E2������ڵ��ڣ�����ѭ���� С�ڵĻ�������ѭ��*/
                        gendo(opr, 0, 11); //���ɱȽ�ָ�i�Ƿ���ڵ���E2��ֵ
                        cx2 = cx;          //����ѭ��������
                        //����������תָ�����ѭ���������ĵ�ַδ֪
                        gendo(jpc, 0, 0);
                        if (sym == dosym) //����ѭ����S
                        {
                            getsymdo;
                            statement(fsys, ptx, lev); //ѭ���崦��
                            //����ѭ����������Ϊ
                            //��ѭ������ȡ������ջ��
                            gendo(lod, lev - table[i].level, table[i].adr);
                            gendo(lit, 0, 1); //������ȡ��ջ��
                            gendo(opr, 0, 3); //ѭ�������Ӳ���
                            //��ջ����ֵ����ѭ������
                            gendo(sto, lev - table[i].level, table[i].adr);
                            gendo(jmp, 0, cx1); //��������ת��ѭ����ʼ��
                            /*����ѭ��������ĵ�ַ��cxΪelse�����ִ�����λ�ã�������ǰ��δ������ת��ַ*/
                            code[cx2].a = cx;
                        }
                        else
                        {
                            error(29); //for���������do
                        }
                        break;
                    }
                }
            }
        }
        else
        {
            error(19); //for�������ֵ��䣬��ֵ������Ǳ�����ȱ�ٱ���
        }
    }

    else
    {
        if (sym == readsym) /* ׼������read��䴦�� */
        {
            getsymdo;
            if (sym != lparen)
            {
                error(34); /* ��ʽ����Ӧ�������� */
            }
            else
            {
                do
                {
                    getsymdo;
                    if (sym == ident)
                    {
                        i = position(id, *ptx);
                    }
                    else
                    {
                        i = 0;
                    }
                    if (i == 0)
                    {
                        error(35);
                    }
                    else if (table[i].kind == charcon) //�ַ���������
                    {
                        gendo(opr, 0, 19);
                        gendo(sto, lev - table[i].level, table[i].adr);
                    }
                    else
                    {
                        gendo(opr, 0, 16);
                        gendo(sto, lev - table[i].level, table[i].adr); /* ���浽����*/
                    }
                    getsymdo;
                } while (sym == comma); /*һ��read���ɶ�������� */
            }
            if (sym != rparen)
            {
                error(33);                /* ��ʽ����Ӧ��������*/
                while (!inset(sym, fsys)) /* �����ȣ�ֱ���յ��ϲ㺯���ĺ������*/
                {
                    getsymdo;
                }
            }
            else
            {
                getsymdo;
            }
        }
        else
        {
            if (sym == writesym) /* ׼������write��䴦����read����*/
            {
                getsymdo;
                if (sym == lparen)
                {
                    do
                    {
                        getsymdo;
                        if (sym == ident)
                        {
                            i = position(id, *ptx);
                            if (i == 0)
                            {
                                error(11); /*����δ�ҵ�*/
                            }
                        }
                        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                        nxtlev[rparen] = true;
                        nxtlev[comma] = true;           /* write�ĺ������Ϊ��or��*/
                        expressiondo(nxtlev, ptx, lev); /* ���ñ��ʽ�����˴���read��ͬ��readΪ��������ֵ*/
                        if (table[i].kind == charcon)   //�ַ������
                        {
                            gendo(opr, 0, 17);
                        }
                        else
                            gendo(opr, 0, 14); /* �������ָ����ջ����ֵ*/
                    } while (sym == comma);
                    if (sym != rparen)
                    {
                        error(33); /* write()ӦΪ�������ʽ*/
                    }
                    else
                    {
                        getsymdo;
                    }
                }
                gendo(opr, 0, 15); /* �������*/
            }
            else
            {
                if (sym == callsym) /* ׼������call��䴦��*/
                {
                    getsymdo;
                    if (sym != ident)
                    {
                        error(14); /*call��ӦΪ��ʶ��*/
                    }
                    else
                    {
                        i = position(id, *ptx);
                        if (i == 0)
                        {
                            error(11); /*����δ�ҵ�*/
                        }
                        else
                        {
                            if (table[i].kind == procedur)
                            {
                                gendo(cal, lev - table[i].level, table[i].adr); /*����callָ��*/
                            }
                            else
                            {
                                error(15); /*call���ʶ��ӦΪ����*/
                            }
                        }
                        getsymdo;
                    }
                }
                else
                {
                    if (sym == ifsym) /*׼������if��䴦��*/
                    {
                        getsymdo;
                        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                        nxtlev[thensym] = true;
                        nxtlev[dosym] = true;          /*�������Ϊthen��do*/
                        conditiondo(nxtlev, ptx, lev); /*�������������߼����㣩����*/
                        if (sym == thensym)
                        {
                            getsymdo;
                        }
                        else
                        {
                            error(16); /*ȱ��then*/
                        }
                        cx1 = cx;                    /*���浱ǰָ���ַ*/
                        gendo(jpc, 0, 0);            /*����������תָ���ת��ַ��д0*/
                        statementdo(fsys, ptx, lev); /*����then������*/

                        //if(sym==semicolon)
                        //{
                        //  getsymdo;
                        //}
                        if (sym == elsesym) /*then�����淢��else*/
                        {
                            getsymdo;
                            cx2 = cx;
                            code[cx1].a = cx + 1; /*cxΪ��ǰ��ָ���ַ��
                                              cx+1��Ϊthen���ִ�к��else����λ�ã������ַ*/
                            gendo(jmp, 0, 0);
                            statementdo(fsys, ptx, lev);
                            code[cx2].a = cx; /*��statement�����cxΪelse�����ִ��
                                                ���λ�ã�������ǰ��δ������ת��ַ�������ַ*/
                        }
                        else
                        {

                            code[cx1].a = cx; /*��statement�����cxΪthen�����ִ��
                                                   ���λ�ã�������ǰ��δ������ת��ַ*/
                        }
                    }
                    else
                    {
                        if (sym == beginsym) /*׼�����ո�����䴦��*/
                        {
                            getsymdo;
                            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                            nxtlev[semicolon] = true;
                            nxtlev[endsym] = true; /*�������Ϊ�ֺŻ�end*/
                            /*ѭ��������䴦������ֱ����һ�����Ų�����俪ʼ���Ż��յ�end*/
                            statementdo(nxtlev, ptx, lev);
                            while (inset(sym, statbegsys) || sym == semicolon)
                            {
                                if (sym == semicolon)
                                {
                                    getsymdo;
                                }
                                else
                                {
                                    error(10); /*ȱ�ٷֺ�*/
                                }
                                statementdo(nxtlev, ptx, lev);
                            }
                            if (sym == endsym)
                            {
                                getsymdo;
                            }
                            else
                            {
                                error(17); /*ȱ��end��ֺ�*/
                            }
                        }
                        else
                        {
                            if (sym == whilesym) /*׼������while��䴦��*/
                            {
                                cx1 = cx; /*�����ж�����������λ��*/
                                getsymdo;
                                memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                                nxtlev[dosym] = true;          /*�������Ϊdo*/
                                conditiondo(nxtlev, ptx, lev); /*������������*/
                                cx2 = cx;                      /*����ѭ����Ľ�������һ��λ��*/
                                gendo(jpc, 0, 0);              /*����������ת��������ѭ���ĵ�ַδ֪*/
                                if (sym == dosym)
                                {
                                    getsymdo;
                                }
                                else
                                {
                                    error(18); /*ȱ��do*/
                                }
                                statementdo(fsys, ptx, lev); /*ѭ����*/
                                gendo(jmp, 0, cx1);          /*��ͷ�����ж�����*/
                                code[cx2].a = cx;            /*��������ѭ���ĵ�ַ����if����*/
                            }
                            else
                            {
                                memset(nxtlev, 0, sizeof(bool) * symnum); /*�������޲��ȼ���*/
                                testdo(fsys, nxtlev, 19);                 /*�������������ȷ��*/
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
/*
*���ʽ����
*/
int expression(bool *fsys, int *ptx, int lev)
{
    enum symbol addop; /*���ڱ���������*/
    bool nxtlev[symnum];
    if (sym == plus || sym == minus) /*��ͷ�������ţ���ʱ��ǰ���ʽ������һ�����Ļ򸺵���*/
    {
        addop = sym; /*���濪ͷ��������*/
        getsymdo;
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        termdo(nxtlev, ptx, lev); /*������*/
        if (addop == minus)
        {
            gendo(opr, 0, 1); /*�����ͷΪ��������ȡ��ָ��*/
        }
    }
    else /*��ʱ���ʽ��������ļӼ�*/
    {
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        termdo(nxtlev, ptx, lev); /*������*/
    }
    while (sym == plus || sym == minus)
    {
        addop = sym;
        getsymdo;
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        termdo(nxtlev, ptx, lev); /*������*/
        if (addop == plus)
        {
            gendo(opr, 0, 2); /*���ɼӷ�ָ��*/
        }
        else
        {
            gendo(opr, 0, 3); /*���ɼ���ָ��*/
        }
    }
    return 0;
}
/*
*���
*/
int term(bool *fsys, int *ptx, int lev)
{
    enum symbol mulop; /*���ڱ���˳�������*/
    bool nxtlev[symnum];
    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
    nxtlev[times] = true;
    nxtlev[slash] = true;
    factordo(nxtlev, ptx, lev); /*��������*/
    while (sym == times || sym == slash)
    {
        mulop = sym;
        getsymdo;
        factordo(nxtlev, ptx, lev);
        if (mulop == times)
        {
            gendo(opr, 0, 4); /*���ɳ˷�ָ��*/
        }
        else
        {
            gendo(opr, 0, 5); /*���ɳ���ָ��*/
        }
    }
    return 0;
}
/*
*���Ӵ���
*/
int factor(bool *fsys, int *ptx, int lev)
{
    int i;
    bool nxtlev[symnum];
    testdo(facbegsys, fsys, 24);  /*������ӵĿ�ʼ���ú�*/
    while (inset(sym, facbegsys)) /*ѭ��ֱ���������ӿ�ʼ����*/
    {
        if (sym == ident) /*����Ϊ�������߱���*/
        {
            i = position(id, *ptx); /*��������*/
            if (i == 0)
            {
                error(11); /*��ʶ��δ����*/
            }
            else
            {
                switch (table[i].kind)
                {
                case constant:                   /*����Ϊ����*/
                    gendo(lit, 0, table[i].val); /*ֱ�Ӱѳ�����ֵ��ջ*/
                    break;
                case variable:                                      /*����Ϊ����*/
                    gendo(lod, lev - table[i].level, table[i].adr); /*�ҵ�������ַ������ֵ��ջ*/
                    break;
                case procedur: /*����Ϊ����*/
                    error(21); /*����Ϊ����*/
                    break;
                case charcon:                                       /*����Ϊ�ַ���*/
                    gendo(lod, lev - table[i].level, table[i].adr); /*�ҵ��ַ��͵�ַ������ֵ��ջ*/
                    break;
                }
            }
            getsymdo;

            if (sym == plusplus)
            {
                gendo(lit, lev - table[i].level, 1);            //��ֵΪ��ջ
                gendo(opr, lev - table[i].level, 2);            //+1,ջ���Ӵ�ջ��
                gendo(sto, lev - table[i].level, table[i].adr); //��ջȡֵ���ڴ�
                gendo(lod, lev - table[i].level, table[i].adr); //ȡֵ��ջ��
                gendo(lit, 0, 1);
                gendo(opr, 0, 3); //ջ��ֵ��
                getsymdo;
            }
            else if (sym == minusminus)
            {
                gendo(lit, lev - table[i].level, 1);            //��ֵΪ��ջ
                gendo(opr, lev - table[i].level, 3);            //-1,ջ���Ӵ�ջ��
                gendo(sto, lev - table[i].level, table[i].adr); //��ջȡֵ���ڴ�
                gendo(lod, lev - table[i].level, table[i].adr); //ȡֵ��ջ��
                gendo(lit, 0, 1);
                gendo(opr, 0, 2); //ջ��ֵ��
                getsymdo;
            }
        }
        else if (sym == plusplus)
        {
            getsymdo;
            if (sym == ident)
            {
                getsymdo;
                i = position(id, *ptx);
                if (i == 0)
                    error(11);
                else
                {
                    if (table[i].kind == variable)
                    {
                        gendo(lod, lev - table[i].level, table[i].adr); //�ҵ�������ַ������ֵ��ջ
                        gendo(lit, 0, 1);                               //������1ȡ��ջ��
                        gendo(opr, 0, 2);                               //ִ�мӲ���
                        gendo(sto, lev - table[i].level, table[i].adr); //��ջȡֵ���ڴ�
                        gendo(lod, lev - table[i].level, table[i].adr); //ȡֵ��ջ��
                    }
                }
            }
        }
        else if (sym == minusminus)
        {
            getsymdo;
            if (sym == ident)
            {
                getsymdo;
                i = position(id, *ptx);
                if (i == 0)
                    error(11);
                else
                {
                    if (table[i].kind == variable)
                    {
                        gendo(lod, lev - table[i].level, table[i].adr); //�ҵ�������ַ������ֵ��ջ
                        gendo(lit, 0, 1);                               //������1ȡ��ջ��
                        gendo(opr, 0, 3);                               //ִ�м�����
                        gendo(sto, lev - table[i].level, table[i].adr); //��ջȡֵ���ڴ�
                        gendo(lod, lev - table[i].level, table[i].adr); //ȡֵ��ջ��
                    }
                }
            }
        }
        else
        {
            if (sym == number) /*����Ϊ��*/
            {
                if (num > amax)
                {
                    error(31);
                    num = 0;
                }
                gendo(lit, 0, num);
                getsymdo;
            }
            else
            {
                if (sym == lparen) /*����Ϊ���ʽ*/
                {
                    getsymdo;
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    nxtlev[rparen] = true;
                    expressiondo(nxtlev, ptx, lev);
                    if (sym == rparen)
                    {
                        getsymdo;
                    }
                    else
                    {
                        error(22); /*ȱ��������*/
                    }
                }
                testdo(fsys, facbegsys, 23); /*���Ӻ��зǷ�����*/
            }
        }
    }
    return 0;
}
/*
 ��������*/
int condition(bool *fsys, int *ptx, int lev)
{
    enum symbol relop;
    bool nxtlev[symnum];
    if (sym == oddsym) /*׼������odd���㴦��*/
    {
        getsymdo;
        expressiondo(fsys, ptx, lev);
        gendo(opr, 0, 6); /*����oddָ��*/
    }
    else
    {
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[eql] = true;
        nxtlev[neq] = true;
        nxtlev[lss] = true;
        nxtlev[leq] = true;
        nxtlev[gtr] = true;
        nxtlev[geq] = true;
        expressiondo(nxtlev, ptx, lev);
        if (sym != eql && sym != neq && sym != lss && sym != leq && sym != gtr && sym != geq)
        {
            error(20);
        }
        else
        {
            relop = sym;
            getsymdo;
            expressiondo(fsys, ptx, lev);
            switch (relop)
            {
            case eql:
                gendo(opr, 0, 8);
                break;
            case neq:
                gendo(opr, 0, 9);
                break;
            case lss:
                gendo(opr, 0, 10);
                break;
            case geq:
                gendo(opr, 0, 11);
                break;
            case gtr:
                gendo(opr, 0, 12);
                break;
            case leq:
                gendo(opr, 0, 13);
                break;
            }
        }
    }
    return 0;
} 

/*���ͳ���*/
void interpret()
{
    int p, b, t;          /*ָ��ָ�룬ָ���ַ��ջ��ָ��*/
    struct instruction i; /*��ŵ�ǰָ��*/
    int s[stacksize];     /*ջ*/
    //double s[stacksize];  //�ĳ�doubule��ջ
    printf("start pl0\n");
    t = 0;
    b = 0;
    p = 0;
    s[0] = s[1] = s[2] = 0;
    do
    {
        i = code[p]; /*����ǰָ��*/
        p++;
        switch (i.f)
        {
        case lit: /*��a��ֵȡ��ջ��*/
            s[t] = i.a;
            t++;
            break;
        case opr: /*���֡��߼�����*/
            switch (i.a)
            {
            case 0:
                t = b;
                p = s[t + 2];
                b = s[t + 1];
                break;
            case 1:
                s[t - 1] = -s[t - 1];
                break;
            case 2:
                t--;
                s[t - 1] = s[t - 1] + s[t];
                break;
            case 3:
                t--;
                s[t - 1] = s[t - 1] - s[t];
                break;
            case 4:
                t--;
                s[t - 1] = s[t - 1] * s[t];
                break;
            case 5:
                t--;
                s[t - 1] = s[t - 1] / s[t];
                break;
            case 6:
                s[t - 1] = s[t - 1] % 2;
                break;
            case 8:
                t--;
                s[t - 1] = (s[t - 1] == s[t]);
                break;
            case 9:
                t--;
                s[t - 1] = (s[t - 1] != s[t]);
                break;
            case 10:
                t--;
                s[t - 1] = (s[t - 1] < s[t]);
                break;
            case 11:
                t--;
                s[t - 1] = (s[t - 1] >= s[t]);
                break;
            case 12:
                t--;
                s[t - 1] = (s[t - 1] > s[t]);
                break;
            case 13:
                t--;
                s[t - 1] = (s[t - 1] <= s[t]);
                break;
            case 14:                    //14�Ų���Ϊ���ջ��ֵ����
                printf("%d", s[t - 1]); //δ�޸ĳ�ʵ���ַ�����ǰ
                fprintf(fa2, "%d", s[t - 1]);

                //printf("%d",(int)s[t-1]);//���ջ��ֵ��ǿ��ת������
                //fprintf(fa2,"%lf\n",s[t-1]);//ͬʱ��ӡ���ļ�

                t--; //ջ������
                break;
            case 15: //15�Ų���Ϊ������в���
                printf("\n");
                fprintf(fa2, "\n");
                break;
            case 16:
                printf("?");
                fprintf(fa2, "?");
                scanf("%d", &(s[t]));
                fprintf(fa2, "%d\n", s[t]);

                //printf("����������:");
                //fprintf(fa2,"����������:");
                //scanf("%lf",&(s[t]));
                //fprintf(fa2,"%lf\n",s[t]);

                t++;
                break;

            case 17: //17�Ų���Ϊ���ջ��ֵ����
                printf("����ַ�:");
                printf("%c\n", s[t - 1]);       //���ջ��ֵ
                fprintf(fa2, "%c\n", s[t - 1]); //ͬʱ��ӡ���ļ�
                t--;                            //ջ������
                break;

                /*case 18://18�Ų���Ϊ���ջ��ֵ����
                        printf("%lf",s[t-1]);//���ջ��ֵ
                        fprintf(fa2,"%lf\n",s[t-1]);//ͬʱ��ӡ���ļ�
                        t--;//ջ������?
                        break;*/

            case 19:                         //19�Ų����ǽ��ܼ���ֵ���뵽ջ��
                printf("���뵥�ַ�:");       //�����ʺ�
                fprintf(fa2, "���뵥�ַ�:"); //ͬʱ������ļ�
                getchar();                   //���������enter
                scanf("%c", &(s[t]));

                //scanf("%s",sta);
                //s[t]=sta[0];//�������

                fprintf(fa2, "%c\n", s[t]); //���û�����ֵ��ӡ���ļ�
                t++;                        //ջ�����ƣ�����ռ�
                break;

                /*case 20://20�Ų����ǽ��ܼ���ֵ���뵽ջ��
                        printf("����˫����������");//�����ʺ�
                        fprintf(fa2,"����˫����������");//ͬʱ������ļ�
                        scanf("%lf",&(s[t]));//�������
                        fprintf(fa2,"%lf\n",s[t]);//���û�����ֵ��ӡ���ļ�
                        t++;//ջ�����ƣ�����ռ�
                        break;*/
            }
            break;
        case lod: /*ȡ��Ե�ǰ���̵����ݻ���ַΪ����ڴ��ֵ��ջ��*/
            s[t] = s[base(i.l, s, b) + i.a];
            t++;
            break;
        case sto: /*ջ����ֵ�浽��Ե�ǰ���̵����ݻ���ַΪ����ڴ�*/
            t--;
            s[base(i.l, s, b) + i.a] = s[t];
            break;
        case cal:                   /*�����ӳ���*/
            s[t] = base(i.l, s, b); /*�������̻���ַ��ջ*/
            s[t + 1] = b;           /*�������̻���ַ��ջ������������base����*/
            s[t + 2] = p;           /*����ǰָ��ָ����ջ*/
            b = t;                  /*�ı����ַָ��ֵΪ�¹��̵Ļ���ַ*/
            p = i.a;                /*��ת*/
            break;
        case inte: /*�����ڴ�*/
            t += i.a;
            break;
        case jmp: /*ֱ����ת*/
            p = i.a;
            break;
        case jpc: /*������ת*/
            t--;
            if (s[t] == 0)
            {
                p = i.a;
            }
            break;
        }
    } while (p != 0);
}
/*ͨ�����̻�ַ����1����̵Ļ�ַ*/
int base(int l, int *s, int b)
{
    int b1;
    b1 = b;
    while (l > 0)
    {
        b1 = s[b1];
        l--;
    }
    return b1;
}
