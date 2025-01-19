import { PikachuCard, PikachuCardProps } from "@/components/pickachu-card";
import Link from "next/link";

export default async function Home() {
  const response = await fetch("http://localhost:8080/all");
  const data = await response.json();

  return (
    <div className="flex flex-wrap justify-center">
      {data.map((item: PikachuCardProps) => (
        <div className="m-2 w-full sm:w-1/2 md:w-1/3 lg:w-1/4" key={item.id}>
          <Link href={`/item/${item.id}`}>
            <PikachuCard key={item.id} {...item} />
          </Link>
        </div>
      ))}
    </div>
  );
}
