export class Scale
{
    constructor(l)
    {
        console.assert(l[0] == "scale");
        for(let i=1;i<l.length;i++)
        {
            let il = l[i];
            switch(il[0])
            {
            case "name":
                this.name = il.slice(1).join(" ");
                break;
            case "spell":
                this.spell = il.slice(1);
                break;
            case "same":
                this.same = il.slice(1).join(" ");
                break;
            }
        }
    }
}