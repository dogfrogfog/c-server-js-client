import { PikachuCard } from "@/components/pickachu-card";

export default async function Page(props: { params: { id: string } }) {
  const params = await props.params;
  const id = params.id;
  const response = await fetch(`http://localhost:8080/item/${id}`);
  const data = await response.json();

  return <PikachuCard {...data} />;
}
